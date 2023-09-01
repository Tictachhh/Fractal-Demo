#include  "SFML/Graphics.hpp" // replace quotes with less than and greater than symbols
#include <iostream>

void PrepareVertexArray(sf::VertexArray& VA_, sf::FloatRect FR_) {

	int cpt = 0;
	for (int x = 0; x < FR_.width; x++) {
		for (int y = 0; y < FR_.height; y++) {
			VA_[cpt].position = sf::Vector2f(float(x), float(y));
			//VA_[cpt].color = sf::Color::Red;
			cpt++;
		}
	}
}

void GenerateJuliasFractal(sf::VertexArray& VA_, sf::FloatRect FR_, sf::Vector2f complexNb_, int precision_, int zoom_, sf::Vector2f dec_) {

	int cpt0 = 0;
	int cpt1 = 0;

	for (double x = 0; x < FR_.width; x++) {
		for (double y = 0; y < FR_.height; y++) {
			double z_r = ((x - double(FR_.width) / 2) + dec_.x) / zoom_;
			double z_i = ((y - double(FR_.height) / 2) + dec_.y) / zoom_;

			cpt0 = 0;

			while (z_r*z_r + z_i*z_i < 4 && cpt0 < precision_) {
				double tmp = z_r;
				z_r = z_r * z_r - z_i * z_i + complexNb_.x;
				z_i = 2 * tmp * z_i + complexNb_.y;
				cpt0++;
			}


			VA_[cpt1].color = sf::Color(0,
				int((255.0 * cpt0) / precision_),
				int((255.0 * cpt0) / precision_));

			cpt1++;


		}
	}

}

int main()
{
    const int width = 1280;
    const int height = 720;

    
    sf::RenderWindow window(sf::VideoMode(width, height), "Fractal Demo");
    sf::Event event;

	int fractalTypeGeneration = 0; 
	//0 -> CPU 
	//1 -> GPU

	const int precision = 300;
	sf::Vector2f complexNb = { float(-0.4), float(0.6) };
	int zoom = 350;
	float x1 = 0.f;
	float y1 = 0.f;


	sf::FloatRect FR(0.f, 0.f, float(width), float(height));
	
	
	//CPU
	sf::VertexArray VA(sf::Points, int(FR.width * FR.height));
	PrepareVertexArray(VA, FR);

	//GPU
	sf::RectangleShape RS(sf::Vector2f(FR.width, FR.height));
	RS.setPosition(sf::Vector2f(0.f, 0.f));

	sf::Shader fragmentShader;

	if (!fragmentShader.loadFromFile("fragmentShader.frag", sf::Shader::Fragment)) {
		std::cout << "Failed" << std::endl;
	}


    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
            }

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				zoom = int(zoom * 1.1);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
				zoom = int(zoom * 0.9);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				complexNb.x-=0.01;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				complexNb.x+=0.01;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				complexNb.y-=0.01;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				complexNb.y+=0.01;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				y1 -= 10 * 400.f / zoom;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				y1 += 10 * 400.f / zoom;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				x1 -= 10 * 400.f / zoom;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				x1 += 10 * 400.f / zoom;
			}

			window.clear();

			if (fractalTypeGeneration == 0) {
				GenerateJuliasFractal(VA, FR, complexNb, precision, zoom, sf::Vector2f{x1,y1});
				window.draw(VA);
			}
			else if (fractalTypeGeneration == 1) {

				fragmentShader.setUniform("resolution", sf::Vector2f(width, height));
				fragmentShader.setUniform("zoom", float(zoom));
				fragmentShader.setUniform("w", FR.width);
				fragmentShader.setUniform("h", FR.height);
				fragmentShader.setUniform("precision_", float(precision));
				fragmentShader.setUniform("complexNb_r", complexNb.x);
				fragmentShader.setUniform("complexNb_i", complexNb.y);
				fragmentShader.setUniform("x1", float((zoom / FR.width) * x1));
				fragmentShader.setUniform("y1", float((zoom / FR.height) * y1));

				window.draw(RS,&fragmentShader);
			}

			window.display();


        }
    }

    return 0;
}
