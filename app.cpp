/*

Joy++ - A general-purpose multimedia engine.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "app.h"

App::App(int screen_width, int screen_height, const char *window_title, float FPS)
{
    this->window_title = window_title;
    this->screen_width = screen_width;
    this->screen_height = screen_height;
    this->FPS = FPS;
}

ALLEGRO_DISPLAY *App::display = nullptr;
ALLEGRO_EVENT_QUEUE *App::event_queue = nullptr;
ALLEGRO_TIMER *App::timer = nullptr;
ALLEGRO_EVENT App::e;
bool App::running = true;

int App::init_all()
{
    // I'm developing on Windows 7, and MinGW does not handle
    // the new fancy alternative for randomness of C++11 properly.
    // So don't blame my oldschool approach, you Unix user.

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

    al_set_window_title(this->display, this->window_title);

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

void App::run()
{
    this->init_all();
    this->register_all();

    while (App::running)
    {
        // Run the plots.

        if (!this->scenes.empty())
        {
            for (auto const &it : this->scenes)
            {
                it.second();
            }
        }
    }

    this->destroy_all();
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

void App::add_scene(Scene scene)
{
    this->scenes[scene.get_name()] = scene.get_plot();
}

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
