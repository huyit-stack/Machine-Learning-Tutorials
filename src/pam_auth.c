#include "pam_auth.h"

#include <security/pam_appl.h>
#include <stdlib.h>
#include <string.h>

typedef struct ConversationContext {
    const char *password;
} ConversationContext;

static int pam_conversation(int num_msg,
                            const struct pam_message **msg,
                            struct pam_response **resp,
                            void *appdata_ptr) {
    if (num_msg <= 0 || msg == NULL || resp == NULL) {
        return PAM_CONV_ERR;
    }

    ConversationContext *ctx = (ConversationContext *)appdata_ptr;
    struct pam_response *responses = calloc((size_t)num_msg, sizeof(struct pam_response));
    if (responses == NULL) {
        return PAM_CONV_ERR;
    }

    for (int i = 0; i < num_msg; i++) {
        responses[i].resp = NULL;
        responses[i].resp_retcode = 0;

        switch (msg[i]->msg_style) {
            case PAM_PROMPT_ECHO_OFF: // password prompt
            case PAM_PROMPT_ECHO_ON:  // generic prompt (rare in this flow)
                if (ctx == NULL || ctx->password == NULL) {
                    free(responses);
                    return PAM_CONV_ERR;
                }
                responses[i].resp = strdup(ctx->password);
                if (responses[i].resp == NULL) {
                    for (int j = 0; j <= i; j++) {
                        if (responses[j].resp) {
                            size_t len = strlen(responses[j].resp);
                            memset(responses[j].resp, 0, len);
                            free(responses[j].resp);
                        }
                    }
                    free(responses);
                    return PAM_CONV_ERR;
                }
                break;
            case PAM_ERROR_MSG:
            case PAM_TEXT_INFO:
                responses[i].resp = NULL; // no response required
                break;
            default:
                for (int j = 0; j < num_msg; j++) {
                    if (responses[j].resp) {
                        size_t len = strlen(responses[j].resp);
                        memset(responses[j].resp, 0, len);
                        free(responses[j].resp);
                    }
                }
                free(responses);
                return PAM_CONV_ERR;
        }
    }

    *resp = responses;
    return PAM_SUCCESS;
}

bool pam_authenticate_user(const char *username, const char *password, char **error_message) {
    if (error_message) {
        *error_message = NULL;
    }

    if (username == NULL || password == NULL) {
        if (error_message) {
            *error_message = strdup("Invalid input: username or password is NULL.");
        }
        return false;
    }

    pam_handle_t *pamh = NULL;
    ConversationContext ctx = { .password = password };
    struct pam_conv conv = { .conv = pam_conversation, .appdata_ptr = &ctx };

    int pam_status = pam_start("login", username, &conv, &pamh);
    if (pam_status != PAM_SUCCESS) {
        if (error_message) {
            *error_message = strdup("Failed to start PAM.");
        }
        return false;
    }

    // Authenticate the user using the provided password
    pam_status = pam_authenticate(pamh, 0);
    if (pam_status == PAM_SUCCESS) {
        // Verify the account is valid (not expired/locked, etc.)
        pam_status = pam_acct_mgmt(pamh, 0);
    }

    bool success = (pam_status == PAM_SUCCESS);

    if (!success && error_message) {
        const char *msg = pam_strerror(pamh, pam_status);
        if (msg && *msg) {
            *error_message = strdup(msg);
        } else {
            *error_message = strdup("Authentication failed.");
        }
    }

    pam_end(pamh, pam_status);
    return success;
}
