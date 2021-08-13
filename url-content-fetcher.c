/*
 * Produced 2021
 * By https://amattu.com/links/github
 * Copy Alec M.
 * License GNU Affero General Public License v3.0
 */

/* Expanded implemntation of
 *  https://stackoverflow.com/a/13906020/4149581
 */

/* Imports */
#include <stdio.h>
#include <curl/curl.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

/* Structures */
struct result {
  size_t size;
  char *body;
};
struct curl_result {
  struct result *header;
  struct result *body;
};

/* Prototypes */
struct curl_result* handle_url(char* url, char* request_type);
size_t write_data(void *ptr, size_t size, size_t nmemb, struct result *data);
int url_specialchars(const char *url);
void print_error(char* error, int exit);
char* strtolower(char* p);

/* Main Function */
int main(int argc, char* argv[])
{
  /* Result data */
  struct curl_result *data = NULL;

  /* Check Arguments */
  if (argc < 2)
    print_error("Bad argument format. Usage:\n\t{{file_name}} <URL> <GET/POST/HEAD>\n", 1);
  if (!url_specialchars(argv[1]))
    print_error("Bad URL format. Usage:\n\t{{file_name}} <URL> <GET/POST/HEAD>\n", 1);

  /* Fetch URL */
  data = handle_url(argv[1], argv[2]);

  if (data) {
    if (data->header)
      printf("%s\n", data->header->body);
    if (data->body)
      printf("%s\n", data->body->body);
  }

  /* Free variables */
  free(data->header);
  free(data->body);
  free(data);

  /* Exit */
  return 0;
}

/* Write the result data to a struct attribute */
size_t write_data(void *ptr, size_t size, size_t nmemb, struct result *data)
{
  /* Variables */
  size_t index = data->size;
  size_t n = (size * nmemb);
  char* tmp;
  data->size += (size * nmemb);
  tmp = realloc(data->body, data->size + 1);

  /* Check allocation */
  if (tmp) {
    data->body = tmp;
  } else {
    if(data->body) {
      free(data->body);
    }
    fprintf(stderr, "Failed to allocate memory.\n");
    return 0;
  }

  memcpy((data->body + index), ptr, n);
  data->body[data->size] = '\0';

  /* Return */
  return size * nmemb;
}

/* Perform cURL request to URL */
struct curl_result* handle_url(char* url, char* request_type)
{
  /* Variables */
  CURL *curl;
  CURLcode res;
  struct curl_result *result = NULL;

  /* Allocate base memory */
  if ((result = malloc(sizeof(struct curl_result))) == NULL) {
     print_error("Could not allocate request result memory.", 1);
     return result;
  } else {
    result->header = malloc(sizeof(struct result));
    result->header->size = 0;
    result->body = malloc(sizeof(struct result));
    result->body->size = 0;
  }
  if ((result->header->body = malloc(4096)) == NULL) {
    print_error("Could not allocate request head memory.", 1);
    return 0;
  } else
    result->header->body[0] = '\0';
  if ((result->body->body = malloc(4096)) == NULL) {
    print_error("Could not allocate request body memory.", 1);
    return 0;
  } else
    result->body->body[0] = '\0';

  /* Setup cURL */
  curl = curl_easy_init();
  if (curl) {
    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Request Type */
    if (strcmp("post", strtolower(request_type)) == 0)
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
    else if (strcmp("head", strtolower(request_type)) == 0)
      curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

    /* Default Handlers */
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, result->header);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, result->body);

    /* Fetch Result */
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      print_error("Failed to fetch URL. Error:\n\t"/*, curl_easy_strerror(res)*/, 0);

    /* Close handle */
    curl_easy_cleanup(curl);
  }

  /* Return result */
  return result;
}

/* Basic URL validation
  https://www.unix.com/programming/186891-url-validation-help-c.html */
int url_specialchars(const char *url)
{
  static const char *nospecial = "0123456789"
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    ".";

  /* Iterate through URL */
  while (*url) {
    if (strchr(nospecial, *url) == NULL)
      return 0;

    url++;
  }

  /* Return success */
  return 1;
}

/* Output application error */
void print_error(char* error, int exit_prg)
{
  /* Output error */
  fprintf(stderr, "%s", error);

  /* Exit program */
  if (exit_prg)
    exit(1);
}

/* Convert string to lowercase */
char* strtolower(char* original_string)
{
  /* Variables */
  int i = 0;
  int string_length = strlen(original_string);
  char *new_string = malloc(string_length + 1);

  /* Iterate through string */
  for (i = 0; i < string_length; i++)
    new_string[i] = tolower(original_string[i]);

  /* Return result */
  return new_string;
}
