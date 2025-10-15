#ifndef PAM_AUTH_H
#define PAM_AUTH_H

#include <stdbool.h>

// Authenticates a user against the system PAM stack (service: "login").
// Returns true on successful authentication, false otherwise.
// On failure, if error_message is not NULL, an allocated string is returned
// describing the error. The caller must free(*error_message).
bool pam_authenticate_user(const char *username, const char *password, char **error_message);

#endif // PAM_AUTH_H
