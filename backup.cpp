// #include <SFML/Graphics.hpp>
// #include <inttypes.h>
// #include <math.h>
//
// //================================================================================
//
// static const unsigned int WIDTH    = 900;
// static const unsigned int HEIGHT   = 800;
// static const unsigned int MAX_ITER = 64;
//
// //================================================================================
//
// uint8_t  get_iter   (float re, float im);
// sf::Color get_color (uint8_t iter);
// void init_pixel_positions  (sf::VertexArray& pixels);
//
// //================================================================================
//
// int main()
// {
//     sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "MANDELBROT");
//
//     sf::VertexArray pixels(sf::Points, WIDTH * HEIGHT);
//     init_pixel_positions(pixels);
//
//     float center_x   = -0.75f;
//     float center_y   = 0.0f;
//     float scale      = 2.5f;
//
//     sf::Clock clock;
//
//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//                 window.close();
//
//             if (event.type == sf::Event::KeyPressed)
//             {
//                 float dif = 0.05f * scale;
//                 switch (event.key.code)
//                 {
//                     case sf::Keyboard::Left:  center_x -= dif; break;
//                     case sf::Keyboard::Right: center_x += dif; break;
//                     case sf::Keyboard::Up:    center_y -= dif; break;
//                     case sf::Keyboard::Down:  center_y += dif; break;
//                     case sf::Keyboard::J:     scale    *= 1.2f; break;
//                     case sf::Keyboard::K:     scale    *= 0.8f; break;
//                     default: break;
//                 }
//             }
//         }
//
//         float x0 = center_x - scale * 0.5f;
//         float dx = scale  / WIDTH;
//         float y0 = center_y - scale * 0.5f;
//         float dy = scale / HEIGHT;
//
//         float y = y0;
//
//         for (uint yi = 0; yi < HEIGHT; ++yi)
//         {
//             float x = x0;
//
//             for (uint xi = 0; xi < WIDTH; ++xi)
//             {
//                 pixels[yi * WIDTH + xi].color = get_color(get_iter(x, y));
//                 x += dx;
//             }
//
//             y += dy;
//         }
//
//         window.clear();
//         window.draw(pixels);
//         window.display();
//
//         printf("\r%.1f", 1.0 / clock.restart().asSeconds());
//         fflush(stdout);
//     }
//
//     return 0;
// }
//
// //================================================================================
//
// void init_pixel_positions(sf::VertexArray& pixels)
// {
//     for (int y = 0; y < HEIGHT; y++)
//         for (int x = 0; x < WIDTH; x++)
//             pixels[y * WIDTH + x].position = sf::Vector2f((float) x, (float) y);
// }
//
// //================================================================================
//
// uint8_t get_iter(float x0, float y0)
// {
//     uint8_t iter = 0;
//     float x = 0, y = 0, tmp = 0;
//
//     while (iter < MAX_ITER && (x*x + y*y) < 100.0f)
//     {
//         tmp = x*x - y*y + x0;
//         y   = x*y + x*y + y0;
//         x   = tmp;
//         iter++;
//     }
//
//     return iter;
// }
//
// //================================================================================
//
// sf::Color get_color(uint8_t iter)
// {
//     if (iter == MAX_ITER)
//         return sf::Color::Black;
//
//     double t = (double) (iter);
//     double r = (t / MAX_ITER + 1/3 + 0.05*(iter%2)) * M_PI_2;
//     double g = (t / MAX_ITER + 7/3 + 0.01*(iter%2)) * M_PI_2;
//     double b = (t / MAX_ITER + 5/3 +    0*(iter%2)) * M_PI_2;
//
//     return sf::Color((uint8_t) (255 * (r - r*r*r/6)),
//                      (uint8_t) (255 * (g - g*g*g/6)),
//                      (uint8_t) (255 * (b - b*b*b/6)));
// }
