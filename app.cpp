#include "app.h"

unsigned int App::get_random_int(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    do
    {
        r = rand();
    } while (r >= limit);

    return min + (r / buckets);
}

App::App(int screen_width, int screen_height, float FPS)
{
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    this->FPS = FPS;
    this->init_all();
    this->register_all();
}

int App::init_all()
{
    this->display = nullptr;
    this->event_queue = nullptr;
    this->timer = nullptr;
    this->running = true;

    srand(time(nullptr));

    if (!al_init())
    {
        cout << "Failed to initialize Allegro." << endl;
        return -1;
    }

    if (!al_init_image_addon())
    {
        cout << "Failed to initialize the image addon." << endl;
        return -1;
    }

    this->display = al_create_display(this->screen_width, this->screen_height);

    if (!this->display)
    {
        cout << "Failed to create the display." << endl;
        return -1;
    }

    if (!al_install_keyboard())
    {
        cout << "Failed to install the keyboard." << endl;
        return -1;
    }

    this->event_queue = al_create_event_queue();

    if (!this->event_queue)
    {
        cout << "Failed to create the event queue." << endl;
        al_destroy_display(this->display);
        return -1;
    }

    this->timer = al_create_timer(0.30 / this->FPS);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();

    return 0;
}

void App::register_all()
{
    al_register_event_source(this->event_queue, al_get_display_event_source(this->display));
    al_register_event_source(this->event_queue, al_get_timer_event_source(this->timer));
    al_start_timer(this->timer);
    al_register_event_source(this->event_queue, al_get_keyboard_event_source());
}

void App::destroy_all()
{
    al_destroy_display(this->display);
    /*
    al_destroy_bitmap(player.bitmaps["UP"]);
    al_destroy_bitmap(player.bitmaps["LEFT"]);
    */
    al_destroy_event_queue(this->event_queue);
}