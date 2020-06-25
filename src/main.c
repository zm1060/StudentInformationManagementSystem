#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "sqlite3.h"

//window
GtkWidget *window_login;
GtkWidget *window_mode;
GtkWidget *window_add;
GtkWidget *window_query;
GtkWidget *window_sort;


//builder
GtkBuilder  *builder_login;
GtkBuilder  *builder_mode;
GtkBuilder  *builder_add;
GtkBuilder  *builder_query;
GtkBuilder  *builder_sort;


//entry
GtkWidget *g_entry_username;
GtkWidget *g_entry_password;

GtkWidget *g_entry_query_id;
GtkWidget *g_entry_query_name;



GtkWidget *g_entry_add_id;
GtkWidget *g_entry_add_name;
GtkWidget *g_entry_add_chinese;
GtkWidget *g_entry_add_maths;
GtkWidget *g_entry_add_english;


//TO LIST ALL
GtkCellRenderer *renderer;
GtkTreeViewColumn *column;
GtkListStore *store;
GtkWidget *window;
GtkWidget *list;

GtkWidget *vbox;
GtkWidget *label;
GtkTreeSelection *selection;

int callback(void *, int, char **, char **);
void initializer_window();

enum {
  LIST_ID,
  LIST_NAME,
  LIST_CHINESE,
  LIST_MATHS,
  LIST_ENGLISH,
  LIST_AVER,
  LIST_TOTAL,
  LIST_RANK,
  N_COLUMNS
};



typedef struct {

    GtkWidget *g_button_login;

} user_widgets;

typedef struct {


    GtkWidget *g_button_save;
    GtkWidget *g_label_add_result;

} save_widgets;

typedef struct {

    GtkWidget *g_button_add;
    GtkWidget *g_button_delete;
    GtkWidget *g_button_update;
    GtkWidget *g_button_search;
    GtkWidget *g_button_analysis;
    GtkWidget *g_button_sort;
    GtkWidget *g_button_show_all;
    GtkWidget *g_button_exit;

}mode_widgets;



typedef struct{
    GtkWidget *g_button_query;
    GtkWidget *g_button_back;
}query_widgets;

typedef struct{
    GtkWidget *g_label_add_result;
}add_widgets;

typedef struct{
    GtkWidget *g_button_TD;
    GtkWidget *g_button_TA;
    GtkWidget *g_button_ID;
    GtkWidget *g_button_IA;
    GtkWidget *g_button_AD;
    GtkWidget *g_button_AA;
    GtkWidget *g_button_C;
    GtkWidget *g_button_M;
    GtkWidget *g_button_E;
}sort_widgets;




typedef struct{
    int Rank;
    long Id;
    char *Name;
    float Chinese;
    float Maths;
    float English;
    float Aver;
    float Total;

}STU;


//string to float
float run_strtof (const char * input)
{
    float output;
    char * end;

    printf ("With strtod, '%s'", input);

    output = strtod (input, & end);

    if (end == input) {
        printf (" is not a valid number.\n");
        return output;
    }
    else {
        printf (" becomes %g\n", output);
        return output;
    }
}

//string to double
double run_strtod (const char * input)
{
    double output;
    char * end;

    printf ("With strtod, '%s'", input);

    output = strtod (input, & end);

    if (end == input) {
        printf (" is not a valid number.\n");
        return output;
    }
    else {
        printf (" becomes %g\n", output);
        return output;
    }
}

//string to long
long run_strtol (const char * input)
{
    long output;
    char * end;

    printf ("With strtod, '%s'", input);

    output = strtod (input, & end);

    if (end == input) {
        printf (" is not a valid number.\n");
        return output;
    }
    else {
        printf (" becomes %ld\n", output);
        return output;
    }
}


int callback(void *model, int argc, char **argv, char **azColName) {
   GtkTreeIter iter;

   for (int i = 0; i < argc; i++) {

      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

   }

   printf("\n");

   // AFTER PRINTING TO CONSOLE FILL THE MODEL WITH THE DATA

   gtk_list_store_append (GTK_LIST_STORE(model), &iter);
   gtk_list_store_set (GTK_LIST_STORE(model), &iter, LIST_ID, argv[0],
         LIST_NAME, argv[1],
         LIST_CHINESE, argv[2],
         LIST_MATHS,argv[3],
         LIST_ENGLISH,argv[4],
         LIST_AVER,argv[5],
         LIST_TOTAL,argv[6],
         LIST_RANK,argv[7],-1);

   return 0;
}

void on_button_show_all_clicked()
{
    //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();

}





//window_mode
//
void on_button_add_clicked (GtkButton *button_add, mode_widgets *widgets)
{
    //gchar *t_username,*t_password;
    //t_username = gtk_entry_get_text (widgets->g_entry_username);
    //t_password = gtk_entry_get_text (widgets->g_entry_password);


    builder_add = gtk_builder_new();

    gtk_builder_add_from_file (builder_add, "glade/window_add.glade", NULL);

    window_add = GTK_WIDGET(gtk_builder_get_object(builder_add, "window_add"));

    gtk_window_set_position(GTK_WINDOW(window_add), GTK_WIN_POS_CENTER);

    add_widgets *ad_widgets =  g_slice_new(add_widgets);
    g_entry_add_id      =   GTK_WIDGET(gtk_builder_get_object(builder_add,"entry_add_id"));
    g_entry_add_name    =   GTK_WIDGET(gtk_builder_get_object(builder_add,"entry_add_name"));
    g_entry_add_chinese =   GTK_WIDGET(gtk_builder_get_object(builder_add,"entry_add_chinese"));
    g_entry_add_maths   =   GTK_WIDGET(gtk_builder_get_object(builder_add,"entry_add_maths"));
    g_entry_add_english =   GTK_WIDGET(gtk_builder_get_object(builder_add,"entry_add_english"));


    gtk_builder_connect_signals(builder_add, ad_widgets);

    g_object_unref(builder_add);

    gtk_widget_show(window_add);
    gtk_main();
}

//window_mode
//
void on_button_search_clicked (GtkButton *button_search, mode_widgets *widgets)
{
    builder_query = gtk_builder_new();

    gtk_builder_add_from_file (builder_query, "glade/window_query.glade", NULL);

    window_query = GTK_WIDGET(gtk_builder_get_object(builder_query, "window_query"));

    query_widgets *qy_widgets   = g_slice_new(query_widgets);
    g_entry_query_id    = GTK_WIDGET(gtk_builder_get_object(builder_query,"entry_query_id"));
    g_entry_query_name  = GTK_WIDGET(gtk_builder_get_object(builder_query,"entry_query_name"));
    qy_widgets->g_button_back       = GTK_WIDGET(gtk_builder_get_object(builder_query,"button_back"));
    qy_widgets->g_button_query      = GTK_WIDGET(gtk_builder_get_object(builder_query,"button_query"));

    gtk_window_set_position(GTK_WINDOW(window_query), GTK_WIN_POS_CENTER);

    gtk_builder_connect_signals(builder_query, qy_widgets);

    g_object_unref(builder_query);

    gtk_widget_show(window_query);
    gtk_main();

}

void on_button_delete_clicked()
{


}

void on_button_update_clicked()
{
    char                   q[999];
    sqlite3*               db;
    //sqlite3_stmt*          stmt;


    const gchar *entry_add_id;
    const gchar *entry_add_name;
    const gchar *entry_add_chinese;
    const gchar *entry_add_maths;
    const gchar *entry_add_english;

    long add_id;
    char *ptr;
    float add_chinese;
    float add_maths;
    float add_english;
    float add_aver;
    float add_total;

    entry_add_id      = gtk_entry_get_text (GTK_ENTRY (g_entry_add_id));
    entry_add_name    = gtk_entry_get_text (GTK_ENTRY (g_entry_add_name));
    entry_add_chinese = gtk_entry_get_text (GTK_ENTRY (g_entry_add_chinese));
    entry_add_maths   = gtk_entry_get_text (GTK_ENTRY (g_entry_add_maths));
    entry_add_english = gtk_entry_get_text (GTK_ENTRY (g_entry_add_english));

    add_id  = strtol(entry_add_id, &ptr, 10);
    add_chinese = run_strtof(entry_add_chinese);
    add_maths   = run_strtol(entry_add_maths);
    add_english = run_strtof(entry_add_english);

    add_total   = add_chinese + add_english + add_maths;
    add_aver    = add_total/3.0;

    printf("%ld %s %f %f %f %f %f",add_id,entry_add_name,add_chinese,add_maths,add_english,add_aver,add_total);


    q[sizeof q - 1] = '\0';
    snprintf(
        q,
        sizeof q - 1,
        "INSERT INTO student (Id,Name,Chinese,Maths,English,AverScore,TotalScore) VALUES(%ld,'%s', %f, %f, %f,%f,%f)",
        add_id,entry_add_name,add_chinese,add_maths,add_english,add_aver,add_total
    );

    if (sqlite3_open ("sms.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database.\n");
    }

    printf("\nAdd Successlly!!!\n");

    //sqlite3_prepare(db, q, sizeof q, &stmt, NULL);

    //sqlite3_finalize(stmt);
    char *zErrMsg = 0;

    if(sqlite3_exec(db,q,0,0,&zErrMsg)!=SQLITE_OK)
    {
        printf("zErrMsg = %s\n",zErrMsg);
        return 0x0b;
    }



}

void on_button_sort_clicked()
{
    builder_sort = gtk_builder_new();

    gtk_builder_add_from_file (builder_sort, "glade/window_sort.glade", NULL);

    window_sort = GTK_WIDGET(gtk_builder_get_object(builder_sort, "window_sort"));

    gtk_window_set_position(GTK_WINDOW(window_sort), GTK_WIN_POS_CENTER);

    sort_widgets *sr_widgets =  g_slice_new(sort_widgets);

    sr_widgets->g_button_TA =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_TA"));
    sr_widgets->g_button_TD =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_TD"));
    sr_widgets->g_button_AA =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_AA"));
    sr_widgets->g_button_AD =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_AD"));
    sr_widgets->g_button_IA =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_IA"));
    sr_widgets->g_button_ID =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_ID"));
    sr_widgets->g_button_C  =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_C"));
    sr_widgets->g_button_M  =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_M"));
    sr_widgets->g_button_E  =  GTK_WIDGET(gtk_builder_get_object(builder_sort,"button_E"));

    gtk_window_set_position(GTK_WINDOW(window_sort), GTK_WIN_POS_CENTER);

    gtk_builder_connect_signals(builder_sort, sr_widgets);

    g_object_unref(builder_sort);

    gtk_widget_show(window_sort);
    gtk_main();

}


void on_button_analysis_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT  count(*) As \"Number of Passed\" FROM student WHERE Chinese>=60;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

//window_login
void on_button_login_clicked(GtkButton *button_login, GtkEntry *widgets)
{
    char                   q[999];
    sqlite3*               db;
    sqlite3_stmt*          stmt;
    int                    row = 0;
    int                    bytes;
    const unsigned char*   text;

    const gchar *entry_username;
    const gchar *entry_password;

    entry_username = gtk_entry_get_text (GTK_ENTRY (g_entry_username));
    entry_password = gtk_entry_get_text (GTK_ENTRY (g_entry_password));
    printf("Your input username:%s \n Your input password:%s",entry_username,entry_password);

    q[sizeof q - 1] = '\0';
    snprintf(
        q,
        sizeof q - 1,
        "SELECT * FROM user WHERE username = '%s'",
        entry_username
    );

    if (sqlite3_open ("sms.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Error opening database.\n");
    }

    printf("\nQuery: %s\n", q);

    sqlite3_prepare(db, q, sizeof q, &stmt, NULL);

    int done = 0;
    while (!done) {
        printf("In select while\n");
        switch (sqlite3_step (stmt)) {
        case SQLITE_ROW:
            bytes = sqlite3_column_bytes(stmt, 0);
            text  = sqlite3_column_text(stmt, 1);
            printf ("Password is: %s (%d bytes)\n",text, bytes);
            row++;
            break;

        case SQLITE_DONE:
            done = 1;
            break;

        default:
            fprintf(stderr, "Failed.\n");
            break;
        }
    }

    sqlite3_finalize(stmt);



    if(strcmp((const char *)text,entry_password)==0)
    {
        mode_widgets *mo_widgets = g_slice_new(mode_widgets);

        builder_mode = gtk_builder_new();

        gtk_builder_add_from_file (builder_mode, "glade/window_mode.glade", NULL);

        window_mode = GTK_WIDGET(gtk_builder_get_object(builder_mode, "window_mode"));

        mo_widgets->g_button_add       = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_add"));
        mo_widgets->g_button_delete    = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_delete"));
        mo_widgets->g_button_update    = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_update"));
        mo_widgets->g_button_search    = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_search"));
        mo_widgets->g_button_analysis  = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_analysis"));
        mo_widgets->g_button_sort      = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_sort"));
        mo_widgets->g_button_show_all  = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_show_all"));
        mo_widgets->g_button_exit      = GTK_WIDGET(gtk_builder_get_object(builder_mode,"button_exit"));

        gtk_window_set_position(GTK_WINDOW(window_mode), GTK_WIN_POS_CENTER);
        // signal connect
        gtk_builder_connect_signals(builder_mode, mo_widgets);

        g_object_unref(builder_mode);

        gtk_widget_show(window_mode);

        gtk_main();
    }
    else
    {
        GtkWidget *dialog;
        GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_dialog_new_with_buttons ("Wrong Password!!!",
                                      NULL,
                                      flags,
                                      "OK",
                                      GTK_RESPONSE_ACCEPT,
                                      "Cancel",
                                      GTK_RESPONSE_REJECT,
                                      NULL);

        gtk_dialog_run((GtkDialog *)dialog);

        switch (flags)
        {
            case GTK_RESPONSE_ACCEPT:
                 gtk_widget_destroy (dialog);
                 break;
            default:
                 gtk_widget_destroy (dialog);
                 break;
        }
    }


}

//
void on_button_TD_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }
;

   char *sql = "SELECT * FROM student  ORDER BY TotalScore DESC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

void on_button_TA_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY TotalScore ASC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}


void on_button_AD_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY AverScore DESC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

void on_button_AA_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY AverScore ASC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

void on_button_ID_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY Id DESC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

void on_button_IA_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY Id ASC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}


void on_button_C_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY Chinese DESC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}


void on_button_M_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY Maths DESC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

void on_button_E_clicked()
{
        //just show no handle
    store = gtk_list_store_new (N_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING,
                              G_TYPE_STRING);

   sqlite3 *db;
   char *err_msg = 0;

   int rc = sqlite3_open("sms.db", &db);

   if (rc != SQLITE_OK) {

       fprintf(stderr, "Cannot open database: %s\n",
               sqlite3_errmsg(db));
       sqlite3_close(db);

   }

   char *sql = "SELECT * FROM student  ORDER BY English DESC;";

   rc = sqlite3_exec(db, sql, callback, store, &err_msg);

   if (rc != SQLITE_OK ) {

      fprintf(stderr, "Failed to select data\n");
      fprintf(stderr, "SQL error: %s\n", err_msg);

      sqlite3_free(err_msg);
      sqlite3_close(db);
   }

   sqlite3_close(db);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   list = gtk_tree_view_new();

   // CREATE  8  COLUMNS WITH TEXT CELL RENDERERS

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ID",
          renderer, "text", LIST_ID, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("NAME",
          renderer, "text", LIST_NAME, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("CHINESE",
          renderer, "text", LIST_CHINESE, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("MATHS",
          renderer, "text", LIST_MATHS, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("ENGLISH",
          renderer, "text", LIST_ENGLISH, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("AVER",
          renderer, "text", LIST_AVER, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("TOTAL",
          renderer, "text", LIST_TOTAL, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

   renderer = gtk_cell_renderer_text_new ();
   column = gtk_tree_view_column_new_with_attributes("RANK",
          renderer, "text", LIST_RANK, NULL);
   gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);



   // SET THE TREE VIEW MODEL
   gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

   // TREEVIEW WILL KEEP A REFERENCE SO DECREASE REFCOUNT
   g_object_unref(store);

   // SETUP THE UI
   gtk_window_set_title(GTK_WINDOW(window), "List view");
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_container_set_border_width(GTK_CONTAINER(window), 10);
   gtk_window_set_default_size(GTK_WINDOW(window), 270, 250);

   gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), FALSE);

   vbox = gtk_box_new(FALSE, 0);

   //gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 5);
   label = gtk_label_new("");
   gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 5);

   gtk_container_add(GTK_CONTAINER(window), vbox);

   g_signal_connect(G_OBJECT (window), "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   gtk_widget_show_all(window);

   gtk_main();


}

//wiondow_query
void on_button_back_clicked()
{
    gtk_widget_destroy (window_query);
}

//window_add
void on_button_cancel_clicked()
{
    gtk_widget_destroy (window_add);
}

//window_mode
void on_button_exit_clicked()
{
    gtk_widget_destroy (window_mode);
}


//window_login
void on_button_quit_clicked()
{
    gtk_main_quit();
    gtk_widget_destroy(window_login);
}

// called when window is closed
void on_windows_login_destroy()
{
    gtk_main_quit();
}



/*  To Initialize
****************************************************************************************

*/
void initialize_window()
{
    builder_login = gtk_builder_new();
    gtk_builder_add_from_file (builder_login, "glade/window_login.glade", NULL);

    window_login = GTK_WIDGET(gtk_builder_get_object(builder_login, "window_login"));

    gtk_builder_connect_signals(builder_login, window_login);

    user_widgets *lo_widgets = g_slice_new(user_widgets);

    //get pointer
    lo_widgets->g_button_login   = GTK_WIDGET(gtk_builder_get_object(builder_login,"button_login"));
    g_entry_username = GTK_WIDGET(gtk_builder_get_object(builder_login,"entry_username"));
    g_entry_password = GTK_WIDGET(gtk_builder_get_object(builder_login,"entry_password"));


    gtk_window_set_position(GTK_WINDOW(window_login), GTK_WIN_POS_CENTER);


    // signal connect
    gtk_builder_connect_signals(builder_login, lo_widgets);

    g_object_unref(builder_login);

    gtk_widget_show(window_login);
    gtk_main();

}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    initialize_window();

    return 0;
}
