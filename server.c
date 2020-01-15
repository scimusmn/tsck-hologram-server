#include "server.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void print_settings(settings*);
static void update_settings(struct mg_connection*, struct http_message*);
static void handle_ssi_call(struct mg_connection*, const char*);
static void event_handler(struct mg_connection*, int, void*);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static settings* s;
static const char* http_port = "8000";
static struct mg_serve_http_opts http_server_options;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void launch_server(int argc, char** argv, settings* s_, int* running) {
  s = s_;
  
  struct mg_mgr event_manager;
  struct mg_connection* connection;
  char *p, path[512];

  mg_mgr_init(&event_manager, NULL);
  connection = mg_bind(&event_manager, http_port, event_handler);
  mg_set_protocol_http_websocket(connection);

  http_server_options.document_root = "./web_root";
  http_server_options.auth_domain = "example.com";

  if (argc > 0 && (p = strrchr(argv[0], '/'))) {
    snprintf(path, sizeof(path), "%.*s/web_root", (int) (p - argv[0]), argv[0]);
    http_server_options.document_root = path;
  }

  while(*running) {
    mg_mgr_poll(&event_manager,1000);
  }
  mg_mgr_free(&event_manager);

}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void print_settings(settings* s) {
  printf(" == SETTINGS == \nDistance: ( %f, %f, %f, %f )\nSize:     ( %f, %f, %f, %f )\nRotation: ( %f, %f, %f, %f )\n\n",
         s->distance1,
         s->distance2,
         s->distance3,
         s->distance4,

         s->size1,
         s->size2,
         s->size3,
         s->size4,

         s->rotation1,
         s->rotation2,
         s->rotation3,
         s->rotation4);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void update_settings(struct mg_connection* connection,
                            struct http_message* message) {
  char tmp[32];
  mg_get_http_var(&message->body, "distance1", tmp, sizeof(tmp));
  s->distance1 = atof(tmp);
  mg_get_http_var(&message->body, "distance2", tmp, sizeof(tmp));
  s->distance2 = atof(tmp);
  mg_get_http_var(&message->body, "distance3", tmp, sizeof(tmp));
  s->distance3 = atof(tmp);
  mg_get_http_var(&message->body, "distance4", tmp, sizeof(tmp));
  s->distance4 = atof(tmp);

  mg_get_http_var(&message->body, "size1", tmp, sizeof(tmp));
  s->size1 = atof(tmp);
  mg_get_http_var(&message->body, "size2", tmp, sizeof(tmp));
  s->size2 = atof(tmp);
  mg_get_http_var(&message->body, "size3", tmp, sizeof(tmp));
  s->size3 = atof(tmp);
  mg_get_http_var(&message->body, "size4", tmp, sizeof(tmp));
  s->size4 = atof(tmp);

  mg_get_http_var(&message->body, "rotation1", tmp, sizeof(tmp));
  s->rotation1 = atof(tmp);
  mg_get_http_var(&message->body, "rotation2", tmp, sizeof(tmp));
  s->rotation2 = atof(tmp);
  mg_get_http_var(&message->body, "rotation3", tmp, sizeof(tmp));
  s->rotation3 = atof(tmp);
  mg_get_http_var(&message->body, "rotation4", tmp, sizeof(tmp));
  s->rotation4 = atof(tmp);

  print_settings(s);
  
  // respond
  mg_printf(connection, "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\nContent-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n\r\n%.*s",
            (unsigned long) message->body.len,
            (int) message->body.len,
            message->body.p);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

static void handle_ssi_call(struct mg_connection* connection,
                            const char* parameter) {
  char tmp[32];
  if (strcmp(parameter, "distance1") == 0) {
    mg_printf_html_escape(connection, "%f", s->distance1);
  }
  else if (strcmp(parameter, "distance2") == 0) {
    mg_printf_html_escape(connection, "%f", s->distance2);
  }
  else if (strcmp(parameter, "distance3") == 0) {
    mg_printf_html_escape(connection, "%f", s->distance3);
  }
  else if (strcmp(parameter, "distance4") == 0) {
    mg_printf_html_escape(connection, "%f", s->distance4);
  }
  
  else if (strcmp(parameter, "size1") == 0) {
    mg_printf_html_escape(connection, "%f", s->size1);
  }
  else if (strcmp(parameter, "size2") == 0) {
    mg_printf_html_escape(connection, "%f", s->size2);
  }
  else if (strcmp(parameter, "size3") == 0) {
    mg_printf_html_escape(connection, "%f", s->size3);
  }
  else if (strcmp(parameter, "size4") == 0) {
    mg_printf_html_escape(connection, "%f", s->size4);
  }
  
  else if (strcmp(parameter, "rotation1") == 0) {
    mg_printf_html_escape(connection, "%f", s->rotation1);
  }
  else if (strcmp(parameter, "rotation2") == 0) {
    mg_printf_html_escape(connection, "%f", s->rotation2);
  }
  else if (strcmp(parameter, "rotation3") == 0) {
    mg_printf_html_escape(connection, "%f", s->rotation3);
  }
  else if (strcmp(parameter, "rotation4") == 0) {
    mg_printf_html_escape(connection, "%f", s->rotation4);
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
