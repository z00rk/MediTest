

#include <stdio.h>
#include <stdlib.h>


#ifndef DEFUALT_DB_PATH
#define DEFAULT_DB_PATH /tmp/rec.db
#endif

enum e_db_command {
  DB_COMMAND_CREATE,
  DB_COMMAND_DROP,
  DB_COMMAND_SELECT,
  DB_COMMAND_REMOVE,
  DB_COMMAND_INSERT,
  DB_COMMAND_LAST
};

enum e_db_table {
  DB_TABLE_PATIENT,
  DB_TABLE_USER,
  DB_TABLE_ACT,
  DB_TABLE_DENT,
  DB_TABLE_JOINT,
  DB_TABLE_LAST
};

struct s_db_context {
  void *handle;
  char *path;
};

/**
 * Will alloc memory for the return value
 * caller must free it
 **/
char *f_table_id_to_name (enum s_db_table table)
{
  char *tab_name = NULL;

  switch (table) {
    case DB_TABLE_PATIENT:
      tab_name = strdup ("PATIENT");
      break;
    case DB_TABLE_USER:
      tab_name = strdup ("USER");
      break;
    case DB_TABLE_ACT:
      tab_name = strdup ("ACT");
      break;
    case DB_TABLE_DENT:
      tab_name = strdup ("DENT");
      break;
    case DB_TABLE_JOINT
      tab_name = strdup ("JOINTURE");
      break;
    case DB_TABLE_LAST:
    default:
      printf ("unhandled table id\n");
      break;
  }

  return tab_name;
}

struct s_db_context *DB_Init (void)
{
  struct s_db_context *context;

  context = (struct s_db_context*) malloc (sizeof (struct s_db_context));
  memset (context, 0x00, sizeof (struct s_db_context));

  context->path = strdup (DEFAULT_DB_PATH);
  return context;
}

int32_t DB_IsDBPresent (struct s_db_context *context)
{
  if (!context) {
    printf ("error in context\n");
    return -1;
  }

  if (open (context->path) < 0) {
    printf ("no such file or directory\n");
    return -2;
  }
  return 0;
}

/**
 * @TODO : where the extracted data goes ?
 *
 **/
int32_t DB_Select (struct s_db_context *context, enum e_db_table table, char *field, char *condition)
{
  char *table_name = NULL;
  char *request    = NULL;
  char *msg        = NULL;
  int32_t ret_code = -1;

  if (!context || !context->handle || !field) {
    printf ("error in context or in field\n");
    return ret_code
  }

  table_name = f_table_id_to_name (table);
  if (!table_name) {
    printf ("no such table\n");
    return ret_code;
  }


  asprintf (request, "SELECT %s FROM %s WHERE %s", field, table_name, condition);

  if (sqlite3_exec (context->handle, request, 0, 0, &msg) != SQLITE_OK) {
    printf ("error executing sql command\n");
    ret_code = -1
  } else {
    ret_code = 0;
  }

  if (table_name)
    free (table_name);
  if (request)
    free (request);
  if (msg)
    free (msg);

  return 0;
}

int32_t DB_IsTablePresent (struct s_db_context *context, enum e_db_table table)
{
  char *tab_name = NULL;
  char *lookup_req = "SELECT name FROM sqlite_master WHERE type='table' AND name=";
  char *request = NULL;
  char *msg;

  if (!context) {
    printf ("error in context\n");
    return -1;
  }

  tab_name = f_table_id_to_name (table);
  if (!tab_name) {
    printf ("no such table name\n");
    return -1;
  }

  asprintf (request, "%s'%s';", lookup_req, tab_name);

  if (sqlite3_exec (context->handle, request, 0, 0, &msg) != SQLITE_OK) {
    printf ("table not found\n");
    free (tab_name);
    free (request);
    return -1;
  }

  free (tab_name);
  free (request);
  free (msg);

  return 0;
}

int32_t DB_Create (struct s_db_context *context, enum e_db_table)
{
  char *tab_name = NULL;
  char *msg = NULL;

  if (!context || !context->handle) {
    printf ("error in context or in handle \n");
    return -1;
  }

  tab_name = f_table_id_to_name (table);
  if (!table_name) {
    printf ("no such table \n");
    return -1;
  }

  if (sqlite3_exec (context->handle, request_patient, 0, 0, &msg) != SQLITE_OK) {
    printf ("error creating table\n");
    return -1;
  }

  return 0;
}

int32_t DB_GetPatientByName ()
{

  return 0;
}

int32_t DB_GetPatientByTel ()
{

  return 0;
}

int32_t DB_GetPatientByUser ()
{

  return 0;
}

int32_t DB_GetUserName ()
{

  return 0;
}

int32_t DB_GetUserRight ()
{

  return 0;
}

int32_t DB_GetActByPatient ()
{

  return 0;
}

int32_t DB_GetDentByPatient ()
{

  return 0;
}

int32_t DB_AddUser ()
{

  return 0;
}

int32_t DB_AddPatient ()
{

  return 0;
}

int32_t DB_EditPatient ()
{

  return 0;
}

int32_t DB_AddAct ()
{

  return 0;
}

/** @TODO : choose design **/

/**
 * PATIENT : (patient_id INTEGER PRIMARY KEY, numero_dossier TEXT, nom TEXT, prenom TEXT, date DATE, address TEXT, tel TEXT, profession TEXT, next DATE, user_id INTEGER FK)
 * USER : (user_id INTEGER PRIMARY KEY, nom TEXT, hash TEXT, rights INTEGER)
 * ACT  : (act_id INTEGER PRIMARY KEY, desc_acte TEXT, date DATE, dent_id INTEGER FK, reçu INTEGER, patient_id INTEGER FK);
 * DENT : (dent_id INTEGER PRIMARY KEY, nom TEXT, etat SET, patient_id INTEGER)
 * jonction ACT et DENT : (id_jonction INTEGER PRIMARY KEY, dent_id INTEGER FK, act_id INTEGER FK)
 **/

int32_t DB_CreateRequest (int16_t command, int8_t table, char **request, int16_t argc, ...)
{
  int32_t ret_code = 0;
  char *action = NULL;
  char *table_name  = NULL;

  switch (command){
    case DB_COMMAND_CREATE:
      action = strdup ("CREATE TABLE if not exists");
      printf ("creating new table and ignoring %d params\n",argc);
      break;
    case DB_COMMAND_DROP:
      action = strdup ("DROP TABLE if exists");
      printf ("dropping table and ignoring %d params\n",argc);
      break;
    case DB_COMMAND_INSERT:
      action = strdup ("INSERT INTO");
      break;
    case DB_COMMAND_REMOVE:
      break;
    case DB_COMMAND_SELECT
      action = strdup ("SELECT");
    case DB_COMMAND_LAST
    default:
      printf ("unhandled db action\n");
      ret_code = -1;
      break;
  }

  table_name = f_table_id_to_name (table);
  asprint (*request, "%s %s", action, table_name);

  free (action);
  free (table_name);

  return ret_code;
}

int32_t DB_Drop (struct s_db_context *context, enum e_db_table table)
{
  char *table_name = NULL;
  char *request = NULL;
  char *msg;

  table_name = f_table_id_to_name (table);
  if (!table_name) {
    printf ("no such table\n");
    return -1;
  }

  asprintf (request, "DROP TABLE if exists %s", table_name);

  if (sqlite3_exec (context->handle, request, 0, 0, &msg) != SQLITE_OK) {
    free (request);
    free (table_name);
    printf ("error dropping table\n");
    return -1;
  }

  free (request);
  free (table_name);
  free (msg);

  return 0;
}

int32_t DB_Open (struct s_db_context *context)
{
  if (!context) {
    printf ("error in DB context\n");
    return -1;
  }

  if (sqlite3_open (context->path, &context->handle) != SQLITE_OK) {
    printf ("error opening database\n");
    return -1;
  }
  return 0;
}

int32_t DB_Destroy (struct s_db_context *context)
{
  if (!context) {
    printf ("error in DB context\n");
    return -1;
  }

  if (DB_Close (context)) {
    printf ("error when destroying context\n");
  }

  if (context->path) {
    free (context->path);
  }

  free (context);
  return 0;
}

int32_t DB_Close (struct s_db_context *context)
{
  if (!context) {
    printf ("error in DB context\n");
    return -1;
  }

  sqlite3_close (context->handle);
  context->handle = NULL;

  return 0;
}
