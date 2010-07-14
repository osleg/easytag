/* about.c - 2000/05/05 */
/*
 *  EasyTAG - Tag editor for MP3 and Ogg Vorbis files
 *  Copyright (C) 2000-2003  Jerome Couderc <easytag@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <config.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <glib/gi18n-lib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for 'system'
#ifdef ENABLE_MP3
#   include <id3tag.h>
#   ifdef ENABLE_ID3LIB
#   include <id3.h>
#   endif
#endif
#include <errno.h>

#include "about.h"
#include "easytag.h"
#include "misc.h"
#include "msgbox.h"
#include "charset.h"

#ifndef PACKAGE_DATA_DIR
#   include "../pixmaps/EasyTAG_logo.xpm"
#endif

#ifdef WIN32
#   include "win32/win32dep.h"
#endif


/****************
 * Declarations *
 ****************/
static GtkWidget *AboutWindow = NULL;


/**************
 * Prototypes *
 **************/
void About_Window_Key_Press (GtkWidget *window, GdkEvent *event);
void Quit_About_Window (void);
void About_Window_Go_To_Home_Page (void);



/*************
 * Functions *
 *************/
/*
 * Close the window when the 'escape' key is pressed.
 */
void About_Window_Key_Press (GtkWidget *window ATTRIBUTE_UNUSED, GdkEvent *event)
{
    GdkEventKey *kevent;

    if (event && event->type == GDK_KEY_PRESS)
    {
        kevent = (GdkEventKey *)event;
        switch(kevent->keyval)
        {
            case GDK_Escape:
                Quit_About_Window();
                break;
        }
    }
}


void Quit_About_Window (void)
{
    if (AboutWindow)
    {
        gtk_widget_destroy(AboutWindow);
        AboutWindow = (GtkWidget *)NULL;
        /*gtk_widget_set_sensitive(MainWindow,TRUE);*/
    }
}

void Show_About_Window (void)
{
    GtkWidget *AboutNoteBook;
    GtkWidget *Label;
    GtkWidget *Frame;
    GtkWidget *VBox, *hbox;
    GtkWidget *ScrollWindow;
    GtkWidget *TextView;
    GtkTextBuffer *TextBuffer;
    GtkTextIter iter;
    GtkWidget *Button;
    GtkWidget *Logo;
    GdkPixmap *pixmap;
    GdkBitmap *mask;
    gchar  temp[MAX_STRING_LEN];
    gchar *temp_str;
    gint i;
#ifdef PACKAGE_DATA_DIR
    FILE *file;
    gchar *tmp = NULL;
#endif
    gchar *description_text =
            _(""
            "    EasyTAG is an utility for viewing and editing tags for MP3, MP2, "
            "FLAC, Ogg Vorbis, MP4/AAC, MusePack and Monkey's Audio files. Its "
            "simple and nice GTK+ interface makes tagging easier under GNU/Linux."
            "");


    /* Translation contributions */
    gchar *translations_thanks_text [][2]= {
            {"    - Bastian Kleineidam ",           _("(German translation)")},
            {"    - Adrian Bunk ",                  _("(German translation)")},
            {"    - Colin Marquardt ",              _("(German translation)")},
            {"    - Philipp Thomas ",               _("(German translation)")},
            {"    - Sergey Zhumatiy ",              _("(Russian translation)")},
            {"    - Andrey Astafiev ",              _("(Russian translation)")},
            {"    - Vincent van Adrighem ",         _("(Dutch translation)")},
            {"    - Bj�rn Olievier ",               _("(Dutch translation)")},
            {"    - Patrik Israelsson ",            _("(Swedish translation)")},
            {"    - Anders Str�mer ",               _("(Swedish translation)")},
            {"    - Szel Miklos ",                  _("(Hungarian translation)")},
            {"    - Nagy Boldizsar ",               _("(Hungarian translation)")},
            {"    - M�sz�ros Csaba ",               _("(Hungarian translation)")},
            {"    - Cappelletti Lorenzo ",          _("(Italian translation)")},
            {"    - Costantino Ceoldo ",            _("(Italian translation)")},
            {"    - Takeshi Aihana ",               _("(Japanese translation)")},
            {"    - Olexander Kunytsa ",            _("(Ukrainian translation)")},
            {"    - Cawko Xakep ",                  _("(Ukrainian translation)")},
            {"    - Milan Siebenburger ",           _("(Czech translation)")},
            {"    - Zbynek Mrkvicka ",              _("(Czech translation)")},
            {"    - Jaime Serrano Cartagena ",      _("(Spanish translation)")},
            {"    - Fernando M. Bueno Moreno ",     _("(Spanish translation)")},
            {"    - Francisco Javier F. Serrador ", _("(Spanish translation)")},
            {"    - Maciej Kasprzyk ",              _("(Polish translation)")},
            {"    - Pauliuc George ",               _("(Romanian translation)")},
            {"    - Morten Brix Pedersen ",         _("(Danish translation)")},
            {"    - Apollon Oikonomopoulos ",       _("(Greek translation)")},
            {"    - doutor zero ",                  _("(Brazilian Portuguese translation)")},
            {"    - Luchezar P. Petkov ",           _("(Bulgarian translation)")},
            {"    - Yang Jinsong ",                 _("(Chinese translation)")},
            {"    - Yuval Hager ",                  _("(Hebrew translation)")},
            {NULL,NULL}
            };

    /* General contributions */
    gchar *general_thanks_text =
            "    - Daniel Drake (GTK2 port)\n"
            "    - Mihael Vrbanec (GTK2 port)\n"
            "    - Michael Pujos (Win32 port)\n"
            "    - Andrew Shuvalov\n"
            "    - Sergey Zhumatiy\n"
            "    - Kevin Venkiteswaran\n"
            "    - Sheikholeslami Navid\n"
            "    - Marcus Tegel\n"
            "    - Robert\n"
            "    - Lars Bohn\n"
            "    - Patrik Israelsson\n"
            "    - Adrian Bunk\n"
            "    - Alexander D Harkness\n"
            "    - Vaclav Slavik\n"
            "    - Charles Kerr\n"
            "    - Gian\n"
            "    - Thomas Zajic\n"
            "    - Szel Miklos\n"
            "    - Tore Aursand\n"
            "    - Cappelletti Lorenzo\n"
            "    - Colin Marquardt\n"
            "    - Goetz Waschk\n"
            "    - Holger Schemel\n"
            "    - Artur Polaczynski\n"
            "    - Maciej Kasprzyk\n"
            "    - Daniel Pichler\n"
            "    - Santtu Lakkala\n"
            "    - Philipp Thomas\n"
            "    - Tony Mancill\n"
            "    - Pavel Minayev\n"
            "    - Justus Schwartz\n"
            "    - Fredrik Noring\n"
            "    - Guilherme Destefani\n"
            "    - Michael Ihde\n"
            "    - Stewart Whitman\n"
            "    - Javier Kohen\n"
            "    - Alexey Illarionov\n"
            "    - Der Humph\n"
            "    - Emmanuel Brun\n"
            "    - Maarten Maathuis\n"
            "    - Pierre Dumuid\n"
            "";


    /* Check if already opened */
    if (AboutWindow)
    {
        gdk_window_raise(AboutWindow->window);
        return;
    }

    /* Dialog window */
    AboutWindow = gtk_dialog_new();

    /* Config */
    gtk_window_set_title(GTK_WINDOW(AboutWindow),_("About..."));
    gtk_window_set_position(GTK_WINDOW(AboutWindow),GTK_WIN_POS_CENTER);

    /* Signals connection */
    g_signal_connect(G_OBJECT(AboutWindow),"destroy", G_CALLBACK(Quit_About_Window),NULL);
    g_signal_connect(G_OBJECT(AboutWindow),"delete_event",G_CALLBACK(Quit_About_Window),NULL);
    g_signal_connect(G_OBJECT(AboutWindow),"key_press_event", G_CALLBACK(About_Window_Key_Press),NULL);

     /* The NoteBook */
    AboutNoteBook = gtk_notebook_new();
    gtk_notebook_popup_enable(GTK_NOTEBOOK(AboutNoteBook));
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(AboutWindow)->vbox),AboutNoteBook,TRUE,TRUE,0);


    /*
     * Tab for common informations
     */

    Label = gtk_label_new(_("About"));
    Frame = gtk_frame_new(NULL);
    gtk_notebook_append_page (GTK_NOTEBOOK(AboutNoteBook),Frame,Label);
    gtk_container_set_border_width(GTK_CONTAINER(Frame), 2);

    VBox = gtk_vbox_new(FALSE,0);
    gtk_container_add(GTK_CONTAINER(Frame),VBox);

    /* EasyTAG Logo */
    gtk_widget_realize(AboutWindow);

#ifdef PACKAGE_DATA_DIR
    pixmap = gdk_pixmap_create_from_xpm(AboutWindow->window,&mask,NULL,PACKAGE_DATA_DIR"/EasyTAG_logo.xpm");
#else
    pixmap = gdk_pixmap_create_from_xpm_d(AboutWindow->window,&mask,NULL,EasyTAG_logo_xpm);
#endif

    if (pixmap)
    {
        Logo = gtk_image_new_from_pixmap(pixmap, mask);
        g_object_unref(pixmap);
        g_object_unref(mask);
        gtk_box_pack_start(GTK_BOX(VBox),Logo,FALSE,TRUE,0);
        gtk_misc_set_padding(GTK_MISC(Logo),2,2);
    }

    /* Infos */
    Label = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(Label),"<b>"APPNAME" "VERSION"</b>");
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);

    sprintf(temp,_("(compiled: %s)"),__DATE__);
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);

#ifdef ENABLE_MP3 // FIX ME : should separate gtk and id3lib
    sprintf(temp,_("(using: GTK+ %d.%d.%d)"),GTK_MAJOR_VERSION,GTK_MINOR_VERSION,
        GTK_MICRO_VERSION);
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

#ifdef ENABLE_MP3
    sprintf(temp, _("(MP3 file support enabled using: libid3tag %s)"), ID3_VERSION);
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#if (defined ENABLE_ID3LIB) 
    sprintf(temp, _("(ID3v2.3 tags support enabled using: id3lib %d.%d.%d)"),
        ID3LIB_MAJOR_VERSION, ID3LIB_MINOR_VERSION, ID3LIB_PATCH_VERSION);
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#else
    Label = gtk_label_new(_("(ID3v2.3 tags  support disabled)"));
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif
#else
    sprintf(temp, _("(MP3 file support disabled)"));
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

#ifndef ENABLE_OGG
    Label = gtk_label_new(_("(Ogg Vorbis file support disabled)"));
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

#ifndef ENABLE_SPEEX
    Label = gtk_label_new(_("(Speex file support disabled)"));
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

#ifndef ENABLE_FLAC
    Label = gtk_label_new(_("(FLAC file support disabled)"));
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

#ifndef ENABLE_MP4
    Label = gtk_label_new(_("(MP4/AAC file support disabled)"));
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

#ifndef ENABLE_WAVPACK
    Label = gtk_label_new(_("(WavPack file support disabled)"));
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);
#endif

    /* Insert a blank line */
    Label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);

    sprintf(temp,_("Author: %s"),AUTHOR);
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);

    sprintf(temp,_("E-mail: %s"),EMAIL);
    Label = gtk_label_new(temp);
    gtk_box_pack_start(GTK_BOX(VBox),Label,FALSE,TRUE,0);

    hbox = gtk_hbox_new(FALSE,0);
    gtk_box_pack_start(GTK_BOX(VBox),hbox,FALSE,TRUE,0);
    Label = gtk_label_new(_("Web Page: "));
    gtk_misc_set_alignment(GTK_MISC(Label),1,0.5);
    gtk_box_pack_start(GTK_BOX(hbox),Label,TRUE,TRUE,0);
    Button = gtk_button_new_with_label(WEBPAGE);
    gtk_box_pack_start(GTK_BOX(hbox),Button,TRUE,TRUE,0);
    g_signal_connect(G_OBJECT(Button),"clicked", G_CALLBACK(About_Window_Go_To_Home_Page),NULL);
    gtk_button_set_relief(GTK_BUTTON(Button),GTK_RELIEF_NONE);

    /* Insert a blank line */
    Label = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(VBox),Label,TRUE,TRUE,0);

    Frame = gtk_frame_new(_("Description:"));
    gtk_frame_set_shadow_type(GTK_FRAME(Frame),GTK_SHADOW_IN);
    gtk_box_pack_start(GTK_BOX(VBox),Frame,FALSE,TRUE,0);
    gtk_container_set_border_width(GTK_CONTAINER(Frame), 2);

    Label = gtk_label_new(_(description_text));
    gtk_misc_set_padding(GTK_MISC(Label),2,2);
    gtk_label_set_line_wrap(GTK_LABEL(Label),TRUE);
    gtk_container_add(GTK_CONTAINER(Frame),Label);
    gtk_container_resize_children(GTK_CONTAINER(Frame));


    /*
     * Tab for thanks
     */

    Label = gtk_label_new(_("Thanks"));
    Frame = gtk_frame_new(NULL);
    gtk_notebook_append_page (GTK_NOTEBOOK(AboutNoteBook),Frame,Label);

    ScrollWindow = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(Frame),ScrollWindow);
    gtk_container_set_border_width(GTK_CONTAINER(Frame),2);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ScrollWindow),
                                   GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

    TextBuffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_create_tag(TextBuffer, "italic", "style", PANGO_STYLE_ITALIC, NULL);
    gtk_text_buffer_create_tag(TextBuffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);
    gtk_text_buffer_create_tag(TextBuffer, "underline", "underline", PANGO_UNDERLINE_SINGLE, NULL);
    gtk_text_buffer_create_tag(TextBuffer, "large", "scale", PANGO_SCALE_LARGE, NULL);
    //gtk_text_buffer_create_tag(TextBuffer, "x-large", "scale", PANGO_SCALE_X_LARGE, NULL);
    //gtk_text_buffer_create_tag(TextBuffer, "monospace", "family", "monospace", NULL);

    gtk_text_buffer_get_iter_at_offset(TextBuffer, &iter, 0);

    temp_str = _("Translations:\n");
    gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                             temp_str, -1,
                                             "bold", "underline", "large", NULL);

    for (i=0; translations_thanks_text[i][0]!=NULL; i++)
    {
        // Translator name
        if (!g_utf8_validate(translations_thanks_text[i][0], -1, NULL))
            temp_str = convert_string(translations_thanks_text[i][0], "iso-8859-1", "utf-8",TRUE);
        else
            temp_str = g_strdup(translations_thanks_text[i][0]);
        gtk_text_buffer_insert(TextBuffer, &iter, temp_str, -1);
        g_free(temp_str);

        // Translation language
        if (!g_utf8_validate(translations_thanks_text[i][1], -1, NULL))
            temp_str = convert_string(translations_thanks_text[i][1], "iso-8859-1", "utf-8",TRUE);
        else
            temp_str = g_strdup(translations_thanks_text[i][1]);
        gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter, temp_str, -1,
                                                 "italic", NULL);
        g_free(temp_str);
        gtk_text_buffer_insert(TextBuffer, &iter, "\n", -1);
    }

    gtk_text_buffer_insert(TextBuffer, &iter, "\n", -1);

    temp_str = _("General:\n");
    gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                             temp_str, -1,
                                             "bold", "underline", "large", NULL);
    gtk_text_buffer_insert(TextBuffer, &iter, general_thanks_text, -1);

    TextView = gtk_text_view_new_with_buffer(TextBuffer);
    gtk_container_add(GTK_CONTAINER(ScrollWindow),TextView);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(TextView), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(TextView), GTK_WRAP_WORD);


    /*
     * Tab for ChangeLog
     */

#ifdef PACKAGE_DATA_DIR
    Label = gtk_label_new(_("Changes"));
    Frame = gtk_frame_new(NULL);
    gtk_notebook_append_page (GTK_NOTEBOOK(AboutNoteBook),Frame,Label);

    ScrollWindow = gtk_scrolled_window_new(NULL,NULL);
    gtk_container_add(GTK_CONTAINER(Frame),ScrollWindow);
    gtk_container_set_border_width(GTK_CONTAINER(Frame),2);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(ScrollWindow),
                                   GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

    TextBuffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_get_iter_at_offset(TextBuffer, &iter, 0);
    gtk_text_buffer_create_tag(TextBuffer, "monospace", "family", "monospace", NULL);
    gtk_text_buffer_create_tag(TextBuffer, "red_foreground", "foreground", "red", NULL);
    gtk_text_buffer_create_tag(TextBuffer, "blue_foreground", "foreground", "blue", NULL);
    gtk_text_buffer_create_tag(TextBuffer, "bold", "weight", PANGO_WEIGHT_BOLD, NULL);

    gtk_text_buffer_get_iter_at_offset(TextBuffer, &iter, 0);

    // The file 'ChangeLog' to read
    if ( (file=fopen(PACKAGE_DATA_DIR"/ChangeLog","r"))==0 )
    {
        gchar *msg = g_strdup_printf(_("Can't open file '%s' (%s)\n"),PACKAGE_DATA_DIR"/ChangeLog",g_strerror(errno));
        gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                                 msg, -1,
                                                 "monospace", "red_foreground", NULL);
        g_free(msg);
    } else
    {
        gint first_version = 0;

        while (fgets(temp,sizeof(temp),file))
        {
            if (temp[strlen(temp)-1]=='\n')
                temp[strlen(temp)-1]='\0';
            if (temp[strlen(temp)-1]=='\r')
                temp[strlen(temp)-1]='\0';

            // Convert line to UTF-8
            if (!g_utf8_validate(temp, -1, NULL))
                tmp = convert_string(temp, "iso-8859-1", "utf-8",TRUE);
            else
                tmp = g_strdup(temp);

            if (tmp && tmp[0]!=' ') // If first character is a space => 1rst line after title
            {
                first_version++;
                // To set to bold the title of the version and to red the first version
                if (first_version > 2) // As title takes 2 lines
                    gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                                             tmp, -1,
                                                             "monospace", "bold", NULL);
                else
                    gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                                             tmp, -1,
                                                             "monospace", "bold", "blue_foreground", NULL);
            }else
            {
                if (first_version > 2) // As title takes 2 lines
                    gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                                             tmp, -1,
                                                             "monospace", NULL);
                else
                    gtk_text_buffer_insert_with_tags_by_name(TextBuffer, &iter,
                                                             tmp, -1,
                                                             "monospace", "blue_foreground", NULL);
            }

            gtk_text_buffer_insert(TextBuffer, &iter, "\n", -1);
            g_free(tmp);
        }
        fclose(file);
    }
    TextView = gtk_text_view_new_with_buffer(TextBuffer);
    gtk_container_add(GTK_CONTAINER(ScrollWindow),TextView);
    gtk_text_view_set_editable(GTK_TEXT_VIEW(TextView), FALSE);
    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(TextView), GTK_WRAP_WORD);
#endif


    /*
     * Close Button
     */
    Button = Create_Button_With_Pixmap(BUTTON_CLOSE);
    gtk_box_pack_start(GTK_BOX(GTK_DIALOG(AboutWindow)->action_area),Button,FALSE,FALSE,0);
    g_signal_connect(G_OBJECT(Button),"clicked", G_CALLBACK(Quit_About_Window),NULL);
    GTK_WIDGET_SET_FLAGS(Button,GTK_CAN_DEFAULT);
    gtk_widget_grab_default(Button);


    /* Disable main window */
    /*gtk_widget_set_sensitive(MainWindow,FALSE);*/

    gtk_widget_show_all(AboutWindow);
}


void About_Window_Go_To_Home_Page (void)
{
#ifdef WIN32
    ET_Win32_Notify_Uri(WEBPAGE);
#else
    system("gnome-moz-remote "WEBPAGE);
#endif
}
