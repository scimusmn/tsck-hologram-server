#include "mg/mongoose.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

struct settings {
  char dist1[32];
  char dist2[32];
  char dist3[32];
  char dist4[32];
};

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static const char* http_port = "8000";
static struct mg_serve_http_opts http_server_options;
static struct settings s = { "0", "0", "0", "0" };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void update_settings(struct mg_connection* connection,
                            struct http_message* message) {
  mg_get_http_var(&message->body, "dist1", s.dist1, sizeof(s.dist1));
  mg_get_http_var(&message->body, "dist2", s.dist2, sizeof(s.dist2));
  mg_get_http_var(&message->body, "dist3", s.dist3, sizeof(s.dist3));
  mg_get_http_var(&message->body, "dist4", s.dist4, sizeof(s.dist4));

  printf("received \"%s\"\n",message->message.p);
  printf("Settings: ( %s, %s, %s, %s )\n",
         s.dist1,
         s.dist2,
         s.dist3,
         s.dist4);

  // respond
  mg_printf(connection, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\nContent-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n\r\n%.*s",
            (unsigned long) message->body.len,
            (int) message->body.len,
            message->body.p);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void handle_ssi_call(struct mg_connection* connection,
                            const char* parameter) {
  if (strcmp(parameter, "dist1") == 0) {
    mg_printf_html_escape(connection, "%s", s.dist1);
  }
  else if (strcmp(parameter, "dist1") == 0) {
    mg_printf_html_escape(connection, "%s", s.dist1);
  }
  else if (strcmp(parameter, "dist3") == 0) {
    mg_printf_html_escape(connection, "%s", s.dist3);
  }
  else if (strcmp(parameter, "dist4") == 0) {
    mg_printf_html_escape(connection, "%s", s.dist4);
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void event_handler(struct mg_connection* connection,
                          int event,
                          void* event_data) {
  struct http_message* message = (struct http_message*) event_data;

  switch(event) {
  case MG_EV_HTTP_REQUEST:
    if (mg_vcmp(&message->uri, "/update") == 0) {
      update_settings(connection, message);
    }
    else {
      mg_serve_http(connection, message, http_server_options);
    }
    break;
  case MG_EV_SSI_CALL:
    handle_ssi_call(connection, event_data);
    break;
  default:
    // do nothing
    break;
  }
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main(int argc, char** argv) {
  struct mg_mgr event_manager;
  struct mg_connection* connection;
  char *p, path[512];

  mg_mgr_init(&event_manager, NULL);
  connection = mg_bind(&event_manager, http_port, event_handler);
  mg_set_protocol_http_websocket(connection);
  http_server_options.document_root = "./web_root";
  http_server_options.auth_domain = "example.com";

  if (argc > 0 && (p = strrchr(argv[0], '/'))) {
    snprintf(path, sizeof(path), "%.*s/web_root", (int) (p-argv[0]), argv[0]);
    http_server_options.document_root = path;
  }

  printf("Starting hologram server on port %s\n", http_port);
  for (;;) {
    mg_mgr_poll(&event_manager,1000);
  }
  mg_mgr_free(&event_manager);

  return 0;
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
