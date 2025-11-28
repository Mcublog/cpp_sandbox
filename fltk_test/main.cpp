#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Native_File_Chooser.H>

//>>---------------------- Log control
#define LOG_MODULE_NAME fltkdemo
#define LOG_MODULE_LEVEL (4)
#include "libs/debug/log_libs.h"
//<<----------------------

// Обработчик нажатия кнопки
void button_callback(Fl_Widget* widget, void* data) {
    Fl_Box* box = (Fl_Box*)data;

    // Создаём диалог выбора файла
    Fl_Native_File_Chooser file_chooser;
    file_chooser.title("Выберите файл");
    file_chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);

    // Показываем диалог и проверяем, был ли выбран файл
    switch (file_chooser.show()) {
        case -1: // Ошибка
            box->label("Ошибка при открытии диалога!");
            break;
        case 1: // Пользователь нажал "Отмена"
            box->label("Выбор файла отменён.");
            break;
        default: // Файл выбран
            box->label("Кнопка нажата!");
            LOG_INFO("%s", file_chooser.filename());
            break;
    }

    box->redraw();
}




int main() {
    Fl_Window *window = new Fl_Window(800, 600);
    Fl_Box *box = new Fl_Box(20 * 2, 40 * 2, 300 * 2, 100 * 2, "Hello, Tiny Core Linux!");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD + FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);

    Fl_Button* button = new Fl_Button(100, 100, 100, 30, "Нажми меня");
    button->callback(button_callback, box);

    window->end();
    window->show();
    return Fl::run();
}