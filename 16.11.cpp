#include <iostream>
#include <stdexcept>

namespace
{
  enum Color {RED, GREEN, BLUE};
  class Planar;
  class Colored;
  class Point;
  class Vector;
  class ColoredPoint;
  class Brush;

  class Planar
  {
  public:
    virtual ~Planar() = default;

    virtual std::ostream& info(std::ostream& out) = 0;
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual int abs_sqr() const = 0;
  };

  class Colored
  {
  public:
    virtual ~Colored() = default;

    virtual Color getColor() const = 0;
    virtual void setColor(Color color) = 0;
  };

  class Point: public Planar
  {
  public:
    Point(int x, int y):
      x_(x),
      y_(y)
    {}
    virtual ~Point() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Point";
    }

    virtual int x() const override
    {
      return x_;
    }
    virtual int y() const override
    {
      return y_;
    }
    virtual int abs_sqr() const override
    {
      return x_ * x_ + y_ * y_;
    }
  private:
    int x_, y_;
  };

  class Circle: public Planar
  {
  public:
    Circle(Point center, int radius):
      center_(center),
      radius_(radius)
    {
      if (radius < 0)
      {
        throw std::invalid_argument("Negative radius");
      }
    }

    std::ostream& info(std::ostream& out) override
    {
      return out << "Circle";
    }

    int x() const override
    {
      return center_.x();
    }

    int y() const override
    {
      return center_.y();
    }

    int abs_sqr() const override
    {
      return center_.abs_sqr();
    }

    double sqr() const
    {
      const double pi = 3.141592653589793;
      return pi * radius_ * radius_;
    }

    void move(Point p)
    {
      center_ = p;
    }

    void resize(int r)
    {
      if (r < 0)
      {
        throw std::invalid_argument("Negative radius");
      }
      radius_ = r;
    }

  private:
    Point center_;
    int radius_;
  };

  class ColoredCircle: public Circle, public Colored
  {
  public:
    ColoredCircle(Point center, int radius, Color color):
      Circle(center, radius),
      color_(color)
    {}

    std::ostream& info(std::ostream& out) override
    {
      return out << "Colored circle";
    }

    Color getColor() const override
    {
      return color_;
    }

    void setColor(Color color) override
    {
      color_ = color;
    }

  private:
    Color color_;
  };

  class Vector: public Planar
  {
  public:
    Vector(Point a, Point b):
      a_(a),
      b_(b)
    {}
    virtual ~Vector() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Vector";
    }

    virtual int x() const override
    {
      return b_.x() - a_.x();
    }
    virtual int y() const override
    {
      return b_.y() - a_.y();
    }
    virtual int abs_sqr() const override
    {
      return (x() * x() + y() * y());
    }
  private:
    Point a_, b_;
  };

  class ColoredPoint: public Planar, public Colored
  {
  public:
    ColoredPoint(Point p, Color color):
      p_(p),
      cl_(color)
    {}
    virtual ~ColoredPoint() = default;

    virtual std::ostream& info(std::ostream& out) override
    {
      return out << "Colored point";
    }

    virtual int x() const override
    {
      return p_.x();
    }
    virtual int y() const override
    {
      return p_.y();
    }
    virtual int abs_sqr() const override
    {
      return p_.abs_sqr();
    }
    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Point p_;
    Color cl_;
  };

  class Brush: public Colored
  {
  public:
    Brush(Color color):
      cl_(color)
    {}
    virtual ~Brush() = default;

    virtual Color getColor() const override
    {
      return cl_;
    }
    virtual void setColor(Color color) override
    {
      cl_ = color;
    }
  private:
    Color cl_;
  };
}

std::ostream& print_abs_sqr(std::ostream& out, Planar* pl)
{
  return pl->info(out) << ' ' << pl->abs_sqr();
}

void del(Planar* pls[], size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    delete pls[i];
  }
}

int main()
{
  const size_t cap = 6;
  Planar* pls[cap];
  size_t size = 0;
  try
  {
    pls[size++] = new Point(1, 2);
    pls[size++] = new Point(3, 4);
    pls[size++] = new Vector(Point(1, 2), Point(3, 4));
    pls[size++] = new Vector(Point(3, 4), Point(1, 2));
    pls[size++] = new ColoredPoint(Point(1, 2), Color::RED);
    pls[size++] = new ColoredPoint(Point(3, 4), Color::GREEN);
  }
  catch (const std::exception& e)
  {
    del(pls, size);
    std::cerr << e.what() << '\n';
  }

  for (size_t i = 0; i < size; i++)
  {
    print_abs_sqr(std::cout, pls[i]) << '\n';
  }

  del(pls, size);
}
