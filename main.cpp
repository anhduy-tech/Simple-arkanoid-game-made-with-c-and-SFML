#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

int main()
{
  srand(time(0));

  // Lay do phan giai thuc cua man hinh
  VideoMode desktop = VideoMode::getDesktopMode();
  unsigned int screenWidth = desktop.width;
  unsigned int screenHeight = desktop.height;

  // Tao cua so game
  RenderWindow app(desktop, "Arkanoid!", Style::Fullscreen);
  app.setFramerateLimit(60);

  // Kich thuoc thiet ke goc
  const float designWidth = 520.0f;
  const float designHeight = 450.0f;

  // Tinh toan ti le scale theo man hinh
  float scaleX = screenWidth / designWidth;
  float scaleY = screenHeight / designHeight;
  float globalScale = std::min(scaleX, scaleY);

  // Tai background
  Texture t2;
  if (!t2.loadFromFile("images/background.jpg"))
    return 1;
  Sprite sBackground(t2);

  // Scale background theo toan man hinh
  float bgWidth = t2.getSize().x;
  float bgHeight = t2.getSize().y;
  float bgScaleX = (float)screenWidth / bgWidth;
  float bgScaleY = (float)screenHeight / bgHeight;
  sBackground.setScale(bgScaleX, bgScaleY);
  sBackground.setPosition(0, 0);

  // Tai texture cho bong
  Texture t3;
  if (!t3.loadFromFile("images/ball.png"))
    return 1;
  Sprite sBall(t3);

  // Tai texture cho thanh ngang
  Texture t4;
  if (!t4.loadFromFile("images/paddle.png"))
    return 1;
  Sprite sPaddle(t4);

  // Tai texture cho 5 loai khoi
  Texture blockTextures[5];
  for (int i = 0; i < 5; i++)
  {
    if (!blockTextures[i].loadFromFile("images/block0" + std::to_string(i + 1) + ".png"))
      return 1;
  }

  // Scale thanh ngang theo globalScale
  sPaddle.setScale(globalScale, globalScale);

  // Dat kich thuoc bong la 50x50 pixel
  const float ballSize = 50.0f; // Kich thuoc muc tieu cua bong
  float ballScaleX = ballSize / t3.getSize().x; // Tinh scale dua tren kich thuoc texture goc
  float ballScaleY = ballSize / t3.getSize().y;
  sBall.setScale(ballScaleX, ballScaleY);

  // Vi tri ban dau cua thanh ngang
  float paddleX = 300 * globalScale;
  sPaddle.setPosition(paddleX, 440 * globalScale);

  // Vi tri ban dau cua bong tren thanh ngang
  float x = paddleX + (sPaddle.getGlobalBounds().width - ballSize) / 2;
  float y = 440 * globalScale - ballSize;
  sBall.setPosition(x, y);

  // Tao mang cac khoi va diem so
  Sprite block[1000];
  int blockPoints[1000]; // Luu diem so cua tung khoi
  int n = 0;
  for (int i = 1; i <= 15; i++)
  {
    for (int j = 1; j <= 12; j++)
    {
      int blockType = rand() % 5; // Chon ngau nhien loai khoi (0-4)
      block[n].setTexture(blockTextures[blockType]);
      block[n].setScale(globalScale, globalScale);
      block[n].setPosition(i * 43 * globalScale, j * 20 * globalScale);
      blockPoints[n] = blockType + 1; // Gan diem so (1-5)
      n++;
    }
  }

  // Khoi tao bien toc do
  const float ballSpeed = 6.0f; // Toc do bong (pixel moi frame)
  const float paddleSpeed = 8.0f; // Toc do thanh ngang (pixel moi frame)
  float dx = 0, dy = 0; // Ban dau bong khong di chuyen
  int score = 0;

  // Tai font chu
  Font font;
  if (!font.loadFromFile("Bitcount_Grid_Double/BitcountGridDouble-VariableFont_CRSV,ELSH,ELXP,slnt,wght.ttf"))
    return 1;

  // Hien thi diem so
  Text scoreText;
  scoreText.setFont(font);
  scoreText.setCharacterSize(24 * globalScale);
  scoreText.setFillColor(sf::Color::White);
  scoreText.setPosition(10 * globalScale, 10 * globalScale);

  // Hien thi thong bao game over
  Text gameOverText;
  gameOverText.setFont(font);
  gameOverText.setCharacterSize(60 * globalScale);
  gameOverText.setFillColor(sf::Color::Red);
  gameOverText.setString("GAME OVER!");
  gameOverText.setPosition(130 * globalScale, 180 * globalScale);
  gameOverText.setStyle(Text::Bold);

  // Hien thi huong dan choi lai
  Text replayText;
  replayText.setFont(font);
  replayText.setCharacterSize(10 * globalScale);
  replayText.setFillColor(sf::Color::Blue);
  replayText.setString("Press SPACE to replay or Q to quit");
  replayText.setPosition(130 * globalScale, 250 * globalScale);

  bool gameOver = false;
  bool ballReleased = false;

  // Vong lap chinh
  while (app.isOpen())
  {
    Event e;
    while (app.pollEvent(e))
    {
      if (e.type == Event::Closed)
        app.close();
    }

    if (!gameOver)
    {
      // Di chuyen thanh ngang
      float paddleMove = 0;
      if (Keyboard::isKeyPressed(Keyboard::Right))
      {
        paddleMove = paddleSpeed * globalScale;
        if (!ballReleased)
        {
          ballReleased = true;
          dx = ballSpeed * globalScale; // Dat toc do ngang cho bong
          dy = -ballSpeed * globalScale; // Dat toc do doc cho bong
        }
      }
      if (Keyboard::isKeyPressed(Keyboard::Left))
      {
        paddleMove = -paddleSpeed * globalScale;
        if (!ballReleased)
        {
          ballReleased = true;
          dx = -ballSpeed * globalScale; // Dat toc do ngang cho bong
          dy = -ballSpeed * globalScale; // Dat toc do doc cho bong
        }
      }
      sPaddle.move(paddleMove, 0);

      // Gioi han vi tri thanh ngang trong man hinh
      float paddleWidth = sPaddle.getGlobalBounds().width;
      if (sPaddle.getPosition().x < 0)
        sPaddle.setPosition(0, sPaddle.getPosition().y);
      if (sPaddle.getPosition().x > screenWidth - paddleWidth)
        sPaddle.setPosition(screenWidth - paddleWidth, sPaddle.getPosition().y);

      // Cap nhat vi tri bong neu chua tha
      if (!ballReleased)
      {
        paddleX = sPaddle.getPosition().x;
        x = paddleX + (sPaddle.getGlobalBounds().width - ballSize) / 2;
        y = 440 * globalScale - ballSize;
      }
      else
      {
        // Cap nhat vi tri bong
        x += dx;
        for (int i = 0; i < n; i++)
          if (FloatRect(x + 3, y + 3, ballSize - 6, ballSize - 6).intersects(block[i].getGlobalBounds()))
          {
            block[i].setPosition(-100, 0);
            dx = -dx;
            score += blockPoints[i]; // Cong diem dua tren loai khoi
          }

        y += dy;
        for (int i = 0; i < n; i++)
          if (FloatRect(x + 3, y + 3, ballSize - 6, ballSize - 6).intersects(block[i].getGlobalBounds()))
          {
            block[i].setPosition(-100, 0);
            dy = -dy;
            score += blockPoints[i]; // Cong diem dua tren loai khoi
          }

        // Xu ly va cham voi bien man hinh
        if (x < 0 || x > screenWidth - ballSize)
          dx = -dx;
        if (y < 0)
          dy = -dy;
        if (y > screenHeight)
        {
          gameOver = true;
          ballReleased = false;
          dx = 0;
          dy = 0;
        }

        // Xu ly va cham voi thanh ngang
        if (FloatRect(x, y, ballSize, ballSize).intersects(sPaddle.getGlobalBounds()))
          dy = -(rand() % 5 + 2) * globalScale;
      }

      sBall.setPosition(x, y);

      // Cap nhat diem so
      scoreText.setString("Score: " + std::to_string(score));
    }

    // Ve cac doi tuong
    app.clear();
    app.draw(sBackground);
    app.draw(sBall);
    app.draw(sPaddle);

    for (int i = 0; i < n; i++)
      app.draw(block[i]);

    app.draw(scoreText);

    if (gameOver)
    {
      app.draw(gameOverText);
      app.draw(replayText);
    }

    app.display();

    // Xu ly game over va choi lai
    if (gameOver)
    {
      Event e;
      while (app.pollEvent(e))
      {
        if (e.type == Event::Closed)
          app.close();
        if (e.type == Event::KeyPressed)
        {
          if (e.key.code == Keyboard::Space)
          {
            gameOver = false;
            ballReleased = false;
            score = 0;
            paddleX = 300 * globalScale;
            sPaddle.setPosition(paddleX, 440 * globalScale);
            x = paddleX + (sPaddle.getGlobalBounds().width - ballSize) / 2;
            y = 440 * globalScale - ballSize;
            sBall.setPosition(x, y);
            dx = 0;
            dy = 0;
            n = 0;
            // Tao lai cac khoi voi loai ngau nhien
            for (int i = 1; i <= 15; i++)
              for (int j = 1; j <= 12; j++)
              {
                int blockType = rand() % 5; // Chon ngau nhien loai khoi
                block[n].setTexture(blockTextures[blockType]);
                block[n].setScale(globalScale, globalScale);
                block[n].setPosition(i * 43 * globalScale, j * 20 * globalScale);
                blockPoints[n] = blockType + 1; // Gan diem so (1-5)
                n++;
              }
          }
          else if (e.key.code == Keyboard::Q)
          {
            app.close();
          }
        }
      }
    }
  }

  return 0;
}