#include <iostream>
#include <memory>

using namespace std;

struct IShapeDrawingStrategy
{
    virtual ~IShapeDrawingStrategy() = default;
    virtual void Draw() const = 0;
};

struct RectangleDrawingStrategy : public IShapeDrawingStrategy
{
    virtual void Draw() const override
    {
        cout << "Drawing rectangle." << endl;
    }
};

struct EllipseDrawingStrategy : public IShapeDrawingStrategy
{
    virtual void Draw() const override
    {
        cout << "Drawing ellipse." << endl;
    }
};

///////////////////////////////////////////////////////////////


struct Shape
{
    Shape(const shared_ptr<IShapeDrawingStrategy>& drawingStrategy)
        : m_drawingStrategy(drawingStrategy) {}

    void Draw()
    {
        m_drawingStrategy->Draw();
    }

    void SetDrawingStrategy(const shared_ptr<IShapeDrawingStrategy>& drawingStrategy)
    {
        m_drawingStrategy = drawingStrategy;
    }

private:
    shared_ptr<IShapeDrawingStrategy> m_drawingStrategy;
};

int main()
{
    auto rectangleDrawingStrategy = make_shared<RectangleDrawingStrategy>();
    auto ellipseDrawingStrategy = make_shared<EllipseDrawingStrategy>();

    Shape shape1(rectangleDrawingStrategy);
    Shape shape2(ellipseDrawingStrategy);

    shape1.Draw();
    shape2.Draw();

    shape1.SetDrawingStrategy(ellipseDrawingStrategy);

    shape1.Draw();

    return 0;
}
