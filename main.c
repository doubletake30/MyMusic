#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
GtkWidget *window, *fixed, *notebook, *tab_my_playlist, *Page_playlist, *image, *label_mymusic,*label_tab_playlist,*label_playlist,*drawing_area,*label_all,*label_tab_songs,*tab_my_songs,*Page_songs, *label_songs;
GtkWidget *circle_button;
GtkWidget *box_circle;
GtkWidget *provider, *display, *screen;
GtkWidget *chooser_button;
GtkWidget *scrolled_playlist,*viewport;
int playlist_count = 0;
char playlist_name[512];
char playlist_filename_picture[512];
char saved_playlist_name[512][300];
char saved_image[512][300];
int n=-1;
void on_button_clicked(GtkWidget *widget, gpointer data) {
    // Xử lý khi nhấn vào ảnh hoặc label
    g_print("Đã nhấn vào ảnh hoặc label!\n");
}
void on_entry_changed(GtkWidget *entry, gpointer button) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry));
    if (strlen(text) > 0) {
        gtk_widget_set_sensitive(GTK_WIDGET(button), TRUE);
    } else {
        gtk_widget_set_sensitive(GTK_WIDGET(button), FALSE);
    }
}
gboolean on_draw_circle(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Lấy kích thước của DrawingArea
    gint width, height;
    gtk_widget_get_size_request(widget, &width, &height);

    // Tạo đường tròn
    cairo_arc(cr, width/2.0, height/2.0, 20.5, 0, 2 * G_PI);

    // Tô màu đường tròn
    cairo_set_source_rgb(cr, 217.0/255.0, 217.0/255.0, 217.0/255.0);
    cairo_fill(cr);

    // Vẽ dấu cộng
    cairo_set_source_rgb(cr, 112.0/255.0, 112.0/255.0, 112.0/255.0);
    cairo_move_to(cr, width/2.0 - 10, height/2.0);
    cairo_line_to(cr, width/2.0 + 10, height/2.0);
    cairo_move_to(cr, width/2.0, height/2.0 - 10);
    cairo_line_to(cr, width/2.0, height/2.0 + 10);
    cairo_stroke(cr);

    return TRUE;
}
void create_playlist() {
    // Tạo ảnh và label mới cho playlist
    GtkWidget *label_playlist_name;
    GtkWidget *image_playlist;
    int x_offset = 302; // Khoảng cách giữa hai playlist
    int y_position = 155; // Vị trí y của playlist
    const int limited_text = 15; // Giới hạn chiều dài tên playlist trên label
    int new_x = 35;
    int new_y = y_position;
    // Tạo label và ảnh mới cho playlist
    if (strlen(playlist_name) > limited_text) {
        char *limited_name = g_strndup(playlist_name, limited_text);
        label_playlist_name = gtk_label_new(g_strconcat(limited_name, "...", NULL));
        g_free(limited_name);
    } else {
        label_playlist_name = gtk_label_new(playlist_name);
    }
    gtk_widget_set_size_request(label_playlist_name, 164, 22);
    gtk_misc_set_alignment(GTK_MISC(label_playlist_name), 0, 0.5);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(playlist_filename_picture, NULL);
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 250, 250, GDK_INTERP_BILINEAR);
    image_playlist = gtk_image_new_from_pixbuf(resized_pixbuf);

    // Lưu trữ các widget trong container hiện tại vào một mảng
    GList *children = gtk_container_get_children(GTK_CONTAINER(Page_playlist));
    GtkWidget *widgets[g_list_length(children)];
    int i = 0;
    GList *iter;
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        GtkWidget *widget = GTK_WIDGET(iter->data);
        if (strcmp(gtk_widget_get_name(widget), "label_playlist_name") == 0 || strcmp(gtk_widget_get_name(widget), "image_playlist") == 0) {
            widgets[i] = widget;
            i++;
        }
    }
    g_list_free(children);

    // Kiểm tra số lượng playlist hiện tại
    int num_playlists = i / 2;
    const int max_num_playlists = 4; // Số lượng playlist tối đa mà màn hình có thể hiển thị
    if (num_playlists >= max_num_playlists) {
        int cout = 1;
        int playlist_in_row;
        // Dời các playlist còn lại sang bên phải
        for (i = (num_playlists * 2)-1; i > -1; i -= 2) {
            GtkWidget *label = widgets[i - 1];
            GtkWidget *image = widgets[i];
            cout++;
            int temp = cout / 4;
            if (cout - (temp * 4) == 1) {
                int x=0,y=0;
                gtk_widget_translate_coordinates(label, Page_playlist, 0, 0, &x, &y);
                gtk_fixed_move(GTK_FIXED(Page_playlist), label, new_x, y+  302);
                gtk_widget_translate_coordinates(image, Page_playlist, 0, 0, &x, &y);
                gtk_fixed_move(GTK_FIXED(Page_playlist), image, new_x, y + 302);
            } else {
                gint x, y;
                gtk_widget_translate_coordinates(label, Page_playlist, 0, 0, &x, &y);
                int old_x = x;
                int old_y = y;
                gtk_fixed_move(GTK_FIXED(Page_playlist), label, old_x + 302, old_y);
                gtk_widget_translate_coordinates(image, Page_playlist, 0, 0, &x, &y);
                old_x = x;
                old_y = y;
                gtk_fixed_move(GTK_FIXED(Page_playlist), image, old_x + 302, old_y);
            }
        }


        // Thêm playlist mới vào bên trái
        gtk_widget_set_name(image_playlist, "image_playlist");
        gtk_widget_set_name(label_playlist_name, "label_playlist_name");
        gtk_fixed_put(GTK_FIXED(Page_playlist), label_playlist_name, new_x, new_y+259);
        gtk_fixed_put(GTK_FIXED(Page_playlist), image_playlist, new_x, new_y);
        // Đặt tên cho label và ảnh mới của playlist
        gtk_widget_set_name(label_playlist_name, "label_playlist_name");
        gtk_widget_set_name(image_playlist, "image_playlist");

        // Thêm label và ảnh mới vào container
        gtk_container_add(GTK_CONTAINER(Page_playlist), label_playlist_name);
        gtk_container_add(GTK_CONTAINER(Page_playlist), image_playlist);


    } else {
        // Dịch chuyển các widget đã có sang phải theo khoảng cách xác định
        for (int j = i - 1; j >= 0; j-=2) {
            GtkWidget *label = widgets[j-1];
            GtkWidget *image = widgets[j];
            gint x, y;
            gtk_widget_translate_coordinates(label, Page_playlist, 0, 0, &x, &y);
            int old_x = x;
            int old_y = y;
            gtk_fixed_move(GTK_FIXED(Page_playlist), label, old_x+302, old_y);
            gtk_widget_translate_coordinates(image, Page_playlist, 0, 0, &x, &y);
            old_x = x;
            old_y = y;
            gtk_fixed_move(GTK_FIXED(Page_playlist), image, old_x+302, old_y);
        }

        // Thêm playlist mới vào bên phải của các playlist đã có
        gtk_widget_set_name(image_playlist, "image_playlist");
        gtk_widget_set_name(label_playlist_name, "label_playlist_name");
        gtk_fixed_put(GTK_FIXED(Page_playlist), label_playlist_name, new_x, new_y+259);
        gtk_fixed_put(GTK_FIXED(Page_playlist), image_playlist, new_x, new_y);
        // Đặt tên cho label và ảnh mới của playlist
        gtk_widget_set_name(label_playlist_name, "label_playlist_name");
        gtk_widget_set_name(image_playlist, "image_playlist");

        // Thêm label và ảnh mới vào container
        gtk_container_add(GTK_CONTAINER(Page_playlist), label_playlist_name);
        gtk_container_add(GTK_CONTAINER(Page_playlist), image_playlist);

    }
    gtk_widget_show_all(Page_playlist);

}
void on_create_button_clicked(GtkButton *button, gpointer name_playlist) {
    GtkWidget *dialog, *label_name_playlist,*image_playlist;
    GtkFixed *fixed_container;
    GtkDialogFlags flags;

    // Tạo hộp thoại
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Add Songs",NULL,flags,NULL, NULL,NULL, NULL,NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 1000, 800);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    // Thêm container sử dụng GtkFixed
    fixed_container = GTK_FIXED(gtk_fixed_new());
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), GTK_WIDGET(fixed_container));

    strcpy(playlist_name, gtk_entry_get_text(name_playlist));
    const gchar *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser_button));
    if (filename == NULL) {
        filename = "C:\\Users\\ADMIN\\CLionProjects\\MyMusic\\Picture\\default picture.jpg";
    }
    strcpy(playlist_filename_picture, filename);
    n++;
    strcpy(saved_playlist_name[n],playlist_name);
    strcpy(saved_image[n], playlist_filename_picture);

    //Tạo image
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename, NULL);
    GdkPixbuf *resized_pixbuf = gdk_pixbuf_scale_simple(pixbuf, 250, 250, GDK_INTERP_BILINEAR);
    image_playlist = gtk_image_new_from_pixbuf(resized_pixbuf);
    gtk_fixed_put(GTK_FIXED(fixed_container),image_playlist,29,29);

    //label_name_playlist;
    label_name_playlist = gtk_label_new(playlist_name);
    gtk_label_set_max_width_chars(GTK_LABEL(label_name_playlist), 15);
    gtk_label_set_ellipsize(GTK_LABEL(label_name_playlist), PANGO_ELLIPSIZE_END);
    gtk_fixed_put(fixed_container, label_name_playlist, 29, 288);
    gtk_widget_set_size_request(label_name_playlist, 250, 29);
    gtk_widget_set_name( label_name_playlist, "label_name_playlist");
    gtk_style_context_add_provider(gtk_widget_get_style_context(Page_playlist), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    

    create_playlist();
    gtk_widget_destroy(gtk_widget_get_toplevel(GTK_WIDGET(button)));
    // Hiển thị hộp thoại
    gtk_widget_show_all(dialog);

}
void on_create_playlist_button_clicked(GtkWidget *button, gpointer data) {
    GtkWidget *dialog, *name_label, *name_entry, *create_button, *cancel_button;
    GtkFixed *fixed_container;
    GtkDialogFlags flags;
    // Tạo hộp thoại
    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons("Create Playlist",GTK_WINDOW(data),flags,NULL, NULL,NULL, NULL,NULL);
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 439);
    gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);
    gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

    // Thêm container sử dụng GtkFixed
    fixed_container = GTK_FIXED(gtk_fixed_new());
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), GTK_WIDGET(fixed_container));

    // Tạo label "Tên Playlist:"
    name_label = gtk_label_new("CREATE NEW PLAYLIST");
    gtk_fixed_put(fixed_container, name_label, 124, 30);
    gtk_widget_set_size_request(name_label, 352, 32);
    gtk_widget_set_name( name_label, "name_label");
    gtk_style_context_add_provider(gtk_widget_get_style_context(Page_playlist), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo entry để nhập tên playlist
    name_entry = gtk_entry_new();
    gtk_fixed_put(fixed_container, name_entry, 45, 94);
    gtk_widget_set_size_request(name_entry, 511, 77);
    gtk_entry_set_placeholder_text(GTK_ENTRY(name_entry), "Enter name’s playlist");
    gtk_widget_set_name(name_entry, "name_entry");
    gtk_style_context_add_provider(gtk_widget_get_style_context(name_entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo nút Tạo và định vị nó
    create_button = gtk_button_new_with_label("CREATE");
    gtk_fixed_put(fixed_container,  create_button, 205, 328);
    gtk_widget_set_size_request( create_button, 191, 71);
    gtk_widget_set_name( create_button, "create_button");
    gtk_style_context_add_provider(gtk_widget_get_style_context(create_button), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Mặc định nút Tạo bị mờ đi
    gtk_widget_set_sensitive(create_button, FALSE);

    // Tạo label "Ảnh bìa:"
    GtkWidget *cover_label = gtk_label_new("PICTURE FOR PLAYLIST");
    gtk_fixed_put(fixed_container, cover_label, 182, 194);
    gtk_widget_set_size_request(cover_label, 236, 23);
    gtk_widget_set_name(cover_label, "cover_label");
    gtk_style_context_add_provider(gtk_widget_get_style_context(cover_label), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo GtkFileChooserButton để chọn tệp ảnh
    chooser_button = gtk_file_chooser_button_new("Select an image", GTK_FILE_CHOOSER_ACTION_OPEN);
    gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(chooser_button), g_get_home_dir());
    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(chooser_button), FALSE);
    gtk_fixed_put(fixed_container, chooser_button, 229, 240);
    gtk_widget_set_size_request(chooser_button, 143, 58);

    // Khi nhập liệu thay đổi trong khu nhập tên playlist
    g_signal_connect(G_OBJECT(name_entry), "changed", G_CALLBACK(on_entry_changed), create_button);

    // Khi nhấn nút Tạo
    g_signal_connect(G_OBJECT(create_button), "clicked", G_CALLBACK(on_create_button_clicked),name_entry);

    // Hiển thị hộp thoại
    gtk_widget_show_all(dialog);
}
void create_tab()
{
    // Tạo GtkNotebook và thiết lập kích thước và vị trí cho nó
    notebook = gtk_notebook_new();
    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);
    gtk_widget_set_size_request(notebook, 249, 800);
    gtk_fixed_put(GTK_FIXED(fixed), notebook, 0, 145);
    gtk_notebook_set_show_border((GtkNotebook *) notebook, FALSE);

    //label tab playlist
    label_tab_playlist = gtk_label_new("PLAYLIST");
    gtk_widget_set_size_request(label_tab_playlist, 224, 37);
    gtk_widget_set_name(label_tab_playlist, "label_tab_playlist");
    gtk_style_context_add_provider(gtk_widget_get_style_context(Page_playlist), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo tab "PLAYLIST" và thiết lập kích thước và vị trí cho nó
    tab_my_playlist = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(tab_my_playlist, 249, 37);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_my_playlist,label_tab_playlist);

    //Page my playlist
    Page_playlist = gtk_fixed_new();
    gtk_widget_set_size_request(Page_playlist, 1261, 657);
    gtk_box_pack_start(GTK_BOX(tab_my_playlist), Page_playlist, TRUE, TRUE, 0);
    gtk_widget_set_name(Page_playlist, "Page_playlist");
    gtk_style_context_add_provider(gtk_widget_get_style_context(Page_playlist), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    //label tab songs
    label_tab_songs = gtk_label_new("SONGS");
    gtk_widget_set_size_request(label_tab_songs, 224, 37);
    gtk_widget_set_name(label_tab_songs, "label_tab_songs");
    gtk_style_context_add_provider(gtk_widget_get_style_context(Page_songs), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo tab "SONGS" và thiết lập kích thước và vị trí cho nó
    tab_my_songs = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_size_request(tab_my_songs, 249, 37);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_my_songs,label_tab_songs);

    //Page my songs
    Page_songs = gtk_fixed_new();
    gtk_widget_set_size_request(Page_songs, 1251, 657);
    gtk_box_pack_start(GTK_BOX(tab_my_songs), Page_songs, TRUE, TRUE, 0);
    gtk_widget_set_name(Page_songs, "Page_playlist");
    gtk_style_context_add_provider(gtk_widget_get_style_context(Page_songs), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}
void add_widgets_in_tab_playlist(){
    //label_Playlist
    label_playlist = gtk_label_new("PLAYLIST");
    gtk_widget_set_size_request(label_playlist, 250, 49);
    gtk_fixed_put(GTK_FIXED(Page_playlist), label_playlist, 35, 52);
    gtk_widget_set_name(label_playlist, "label_playlist");
    gtk_style_context_add_provider(gtk_widget_get_style_context(label_playlist), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo DrawingArea
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 41, 41);

    // Tạo button và đặt nó vào hình tròn
    circle_button = gtk_button_new();
    gtk_container_add(GTK_CONTAINER(circle_button), drawing_area);
    gtk_widget_set_name(circle_button, "circle_button");
    gtk_style_context_add_provider(gtk_widget_get_style_context(circle_button), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo box_circle và đặt hình tròn vào vị trí
    box_circle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box_circle), circle_button, FALSE, FALSE, 0);
    gtk_fixed_put(GTK_FIXED(Page_playlist), box_circle, 296, 60);
}
void add_widgets_in_tab_songs(){
    //label_Songs
    label_songs = gtk_label_new("SONGS");
    gtk_widget_set_size_request(label_songs, 250, 49);
    gtk_fixed_put(GTK_FIXED(Page_songs), label_songs, 35, 52);
    gtk_widget_set_name(label_songs, "label_songs");
    gtk_style_context_add_provider(gtk_widget_get_style_context(label_songs), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}
void create_window (){

    // Tạo cửa sổ
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "My music");
    gtk_window_set_default_size(GTK_WINDOW(window), 1500, 800);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


    // Tạo GtkCssProvider và thiết lập các quy tắc CSS
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    // Load tệp CSS
    gtk_css_provider_load_from_path(provider, "C:\\Users\\ADMIN\\CLionProjects\\MyMusic\\UI\\MyMusicCSS.css", NULL);

    // Tạo container GtkFixed
    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    // Tạo khu màu và thiết lập tên của nó để áp dụng quy tắc CSS
    GtkWidget *color_box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(color_box1, "color-box1");
    gtk_widget_set_size_request(color_box1, 249, 800);
    gtk_container_add(GTK_CONTAINER(fixed), color_box1);
    gtk_style_context_add_provider(gtk_widget_get_style_context(color_box1), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo khu màu và thiết lập tên của nó để áp dụng quy tắc CSS
    GtkWidget *color_box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(color_box2, "color-box2");
    gtk_fixed_put(GTK_FIXED(fixed),color_box2,248,0);
    gtk_widget_set_size_request(color_box2, 1261, 850);
    gtk_container_add(GTK_CONTAINER(fixed), color_box2);
    gtk_style_context_add_provider(gtk_widget_get_style_context(color_box2), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    // Tạo ảnh và label
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("C:\\Users\\ADMIN\\CLionProjects\\MyMusic\\Picture\\LOGO.png", NULL);
    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_widget_set_size_request(image, 35, 33);
    gtk_fixed_put(GTK_FIXED(fixed), image, 18, 55);

    //Label Mymusic
    label_mymusic = gtk_label_new("MY MUSIC");
    gtk_widget_set_size_request(label_mymusic, 131, 24);
    gtk_fixed_put(GTK_FIXED(fixed), label_mymusic, 58, 59);
    gtk_widget_set_name(label_mymusic, "label_mymusic");
    gtk_style_context_add_provider(gtk_widget_get_style_context(label_mymusic), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}
void connect_event(){
    // Kết nối sự kiện click vào ảnh và label với hàm xử lý
    g_signal_connect(image, "button-press-event", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(label_mymusic, "button-press-event", G_CALLBACK(on_button_clicked), NULL);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw_circle), NULL);
    g_signal_connect(circle_button, "clicked",G_CALLBACK(on_create_playlist_button_clicked), window);
}
int main(int argc, char *argv[]) {

    gtk_init(&argc, &argv);
    create_window();
    create_tab();
    add_widgets_in_tab_playlist();
    add_widgets_in_tab_songs();

    connect_event();

    //chạy chương trình
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}