#include "vectoreditor.h"
#include <QGridLayout>
#include "graphicclasses.h"
#include "vectorwidget.h"
#include <QGraphicsSceneMouseEvent>

using namespace _VectorScan;

// ================================== VectorScene =====================================
VectorScene::VectorScene(QObject *parent) : QGraphicsScene(parent) {}

// ----------------------------------- stopEdit -----------------------------------
void VectorScene::stopEdit()
{
    for(auto item : items())
    {
        if(dynamic_cast<VectorObject*>(item)->isSelected())
        {
            dynamic_cast<VectorObject*>(item)->stopEdit();
            break;
        }
    }
}

// ----------------------------------- editMode -----------------------------------
EditMode VectorScene::editMode()
{
    return eMode;
}

// ----------------------------------- setParam -----------------------------------
void VectorScene::setParam(ParamVectorColor *data)
{
    vColor.setParam(data);
    for(auto v : items())
    {
        auto temp = dynamic_cast<VectorObject*>(v);
        auto vect = temp->getVector();
        vect.color = vColor.getColor();
        temp->setVector(vect);
        vColor.nextCurVal();
        emit vectorChanged(vect);
    }
    update();

}

// ----------------------------------- setParam -----------------------------------
void VectorScene::setParam(ParamVectorObject *data)
{
    paramVect = data;
    for(auto v : items())
    {
        dynamic_cast<VectorObject*>(v)->setParam(data);
    }
    update();
}

// ----------------------------------- setEditMode -----------------------------------
bool VectorScene::setEditMode(EditMode m)
{
    if(currentObj)
        return false;
    eMode = m;
    for(auto item : items())
    {
        dynamic_cast<VectorObject*>(item)->setEditMode(m);
    }
    return true;
}

// ----------------------------------- mousePressEvent -----------------------------------
void VectorScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(eMode == EditMode::Drawing)
    {
        if(event->buttons() == Qt::MouseButton::LeftButton)
        {
            if(!currentObj)
            {
                Vector v;
                static int id_ = 1;
                v.begin = event->scenePos();
                v.end = v.begin;
                v.color = vColor.getColor();
                v.id = id_++;
                currentObj = new VectorObject(paramVect);
                currentObj->setVector(v);
                currentObj->setMode(VectorObject::Painting);
                addItem(currentObj);

                connect(currentObj, SIGNAL(vectorChanged(Vector const&)),
                        this, SIGNAL(vectorChanged(Vector const&)) );

                emit vectorPainted(v);
            }
            else
            {
                currentObj->setMode(VectorObject::ViewMode::Painted);
                currentObj = nullptr;
                vColor.nextCurVal();
            }
        }
        else if(event->button() == Qt::MouseButton::RightButton)
        {
            if(currentObj)
            {
                currentObj->setSelected(true);
                removeVector();
                currentObj = nullptr;
            }
        }
        this->update();
    }
    QGraphicsScene::mousePressEvent(event);
}

// ----------------------------------- mouseMoveEvent -----------------------------------
void VectorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(currentObj)
    {
        auto v = Vector(currentObj->getVector());
        v.end = event->scenePos();
        currentObj->setVector(v);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

// ----------------------------------- mouseReleaseEvent -----------------------------------
void VectorScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

// ----------------------------------- keyPressEvent -----------------------------------
void VectorScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key::Key_Delete)
    {
        removeVector();
    }
}

// -------------------------------------- removeVector --------------------------------
void VectorScene::removeVector()
{
    for(auto item : items())
    {
        if(dynamic_cast<VectorObject*>(item)->isSelected())
        {
            // ***Сделать добавление цвета удаленного вектора***

            emit vectorRemove(dynamic_cast<VectorObject*>(item)->getVector().color);
            disconnect(dynamic_cast<VectorObject*>(item),
                       SIGNAL(vectorChanged(Vector const&)),
                       this, SIGNAL(vectorChanged(Vector const&)) );
            removeItem(item);
            update();
        }
    }
}

// ================================== VectorObject ====================================
VectorObject::VectorObject(ParamVectorObject* par) : QGraphicsObject(), param(par)
{
    setAcceptHoverEvents(true);
    // Неизвестно почему, но без этой строки не отправляется
    // VectorObject::mouseReleaseEvent
    // Другого эффекта данная строка не производит
    setFlag(QGraphicsObject::ItemIsMovable, true);
}

VectorObject::~VectorObject() {}

// ------------------------------------ boundingRect -------------------------------
QRectF VectorObject::boundingRect() const
{
    QRectF rect;
    int coef = param->rad() + param->circWid() + 5;
    //1. Определяем четверть
    // 1 и 2 четверти
    if(vector.begin.x() <= vector.end.x())
    {
        // 2-я четверть
        if(vector.begin.y() >= vector.end.y())
        {
            rect.setLeft  (mapFromScene(vector.begin).x() - coef);
            rect.setTop   (mapFromScene(vector.begin).y() + coef);
            rect.setRight (mapFromScene(vector.end  ).x() + coef);
            rect.setBottom(mapFromScene(vector.end  ).y() - coef);
        }
        // 1-я четверть
        else
        {
            rect.setLeft  (mapFromScene(vector.begin).x() - coef);
            rect.setTop   (mapFromScene(vector.end  ).y() + coef);
            rect.setRight (mapFromScene(vector.end  ).x() + coef);
            rect.setBottom(mapFromScene(vector.begin).y() - coef);
        }
    }
    // 3-я и 4-я четверти
    else
    {
        // 3-я четверть
        if(vector.begin.y() >= vector.end.y())
        {
            rect.setLeft  (mapFromScene(vector.end  ).x() - coef);
            rect.setTop   (mapFromScene(vector.begin).y() + coef);
            rect.setRight (mapFromScene(vector.begin).x() + coef);
            rect.setBottom(mapFromScene(vector.end  ).y() - coef);
        }
        // 4-я четверть
        else
        {
            rect.setLeft  (mapFromScene(vector.end  ).x() - coef);
            rect.setTop   (mapFromScene(vector.end  ).y() + coef);
            rect.setRight (mapFromScene(vector.begin).x() + coef);
            rect.setBottom(mapFromScene(vector.begin).y() - coef);
        }
    }
    return rect;
}

// ------------------------------------ paint -------------------------------
void VectorObject::paint(QPainter *painter,
                         const QStyleOptionGraphicsItem *option,
                         QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    int alpha;
    QColor selectColor = viewMode == ViewMode::Select ? param->selColorBack() : param->colorBack();

    // На этапе рисования
    if(viewMode == ViewMode::Painting)
    {
        alpha = 127;
    }
    else
    {
        alpha = 255;
    }

    QColor color;
    color = vector.color;
    color.setAlpha(alpha);
    painter->setPen(QPen(selectColor, param->lineWid(), param->styleBack()));
    painter->drawLine(mapFromScene(vector.begin), mapFromScene(vector.end));
    painter->setPen(QPen(color, param->lineWid(), param->styleFront()));
    painter->drawLine(mapFromScene(vector.begin), mapFromScene(vector.end));

    painter->setPen(QPen(QBrush(color), param->circWid(), Qt::PenStyle::SolidLine));

    painter->setBrush(param->colorBeginPoint());
    painter->drawEllipse(mapFromScene(vector.begin), param->rad(), param->rad());

    painter->setBrush(param->colorEndPoint());
    painter->drawEllipse(mapFromScene(vector.end  ), param->rad(), param->rad());
}

// ------------------------------------ setMode -------------------------------
void VectorObject::setMode(VectorObject::ViewMode m)
{
    viewMode = m;
}

// ------------------------------------ setVector -------------------------------
void VectorObject::setVector(const Vector &v)
{
    this->lastVector = this->vector;
    this->vector = v;
    emit vectorChanged(v);
    prepareGeometryChange();
}

// ------------------------------------ getVector -------------------------------
Vector const &VectorObject::getVector() const
{
    return vector;
}

// ------------------------------------ setEditMode -------------------------------
void VectorObject::setEditMode(EditMode mode)
{
    eMode = mode;
    viewMode = ViewMode::Painted;
    setSelected(false);
}

// ------------------------------------ isSelected -------------------------------
bool VectorObject::isSelected()
{
    return viewMode == ViewMode::Select;
}

// ------------------------------------ setSelected -------------------------------
void VectorObject::setSelected(bool sel)
{
    viewMode = sel ? ViewMode::Select : ViewMode::Painted;
    scene()->update();
}

// ----------------------------------- setParam -----------------------------------
void VectorObject::setParam(ParamVectorObject *data)
{
    param = data;
}

// ----------------------------------- stopEdit ----------------------------------
void VectorObject::stopEdit()
{
  //  leftPress = false;
    this->vector = this->lastVector;
}

// ------------------------------------ point1Border -------------------------------
QPainterPath VectorObject::point1Border(bool exp) const
{
    qreal x1 = mapFromScene(vector.begin).x();
    qreal y1 = mapFromScene(vector.begin).y();
    qreal inc = exp ? param->incR() : 1.;
    QPainterPath path;
    path.addEllipse(QPointF(x1, y1), param->rad()*inc, param->rad()*inc);
    return path;
}

// ------------------------------------ point2Border -------------------------------
QPainterPath VectorObject::point2Border(bool exp) const
{
    qreal x2 = mapFromScene(vector.end).x();
    qreal y2 = mapFromScene(vector.end).y();
    qreal inc = exp ? param->incR() : 1.;
    QPainterPath path;
    path.addEllipse(QPointF(x2, y2), param->rad()*inc, param->rad()*inc);
    return path;
}
// ----------------------------------- lineBorder ----------------------------------
QPainterPath VectorObject::lineBorder(bool exp) const
{
    if(vector.begin == vector.end)
        return QPainterPath();

    qreal x1 = mapFromScene(vector.begin).x();
    qreal y1 = mapFromScene(vector.begin).y();
    qreal x2 = mapFromScene(vector.end  ).x();
    qreal y2 = mapFromScene(vector.end  ).y();
    qreal x = x2 - x1;
    qreal y = y2 - y1;
    qreal R = qSqrt(x*x + y*y);

    qreal dx =  y*(param->rad() / R) * (exp ? param->incW() : 1.);
    qreal dy = -x*(param->rad() / R) * (exp ? param->incW() : 1.);

    QPointF p1(x1 - dx, y1 - dy);
    QPointF p2(x1 + dx, y1 + dy);
    QPointF p3(x2 + dx, y2 + dy);
    QPointF p4(x2 - dx, y2 - dy);
    QPainterPath path;
    path.moveTo(p1);
    path.lineTo(p2);
    path.lineTo(p3);
    path.lineTo(p4);
    path.lineTo(p1);
    return path;
}

// ------------------------------------ mousePressEvent -------------------------------
void VectorObject::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(eMode == EditMode::Drawing)
        return;
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        leftPress = true;
    }
    for(auto item : scene()->items())
    {
        dynamic_cast<VectorObject*>(item)->setSelected(false);
    }
    setSelected(true);
    scene()->update();
}

// ------------------------------------ mouseReleaseEvent -------------------------------
void VectorObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(eMode == EditMode::Drawing)
        return;
    if(event->button() == Qt::MouseButton::LeftButton)
    {
        leftPress = false;
    }
    lastHandler = Nothing;
    scene()->update();
}

// ------------------------------------ hoverMoveEvent -------------------------------
void VectorObject::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    auto t = new QGraphicsSceneMouseEvent(QGraphicsSceneMouseEvent::Type::Move);
    // Задаем только это, т.к. больше ничего не используется
    t->setScenePos(event->scenePos());
    t->setLastPos(event->lastPos());
    mouseMoveEvent(t);
}
// --------------------------- mouseMoveEvent --------------------------------------
void VectorObject::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(eMode == EditMode::Drawing)
    {
        this->setCursor(QCursor(Qt::ArrowCursor));
        return;
    }
    // Работаем только в этих режимах
    if(viewMode == ViewMode::Select || viewMode == Painted)
    {
        // Расширяем границы только после выбора нужного элемента
        bool checker = lastHandler == Nothing ? false : leftPress;

        // Определяем, в какой части вектора лежит курсор
        if(this->point1Border(checker).contains(event->scenePos()) &&
                (lastHandler == Nothing || lastHandler == BeginPoint))
        {
            // Курсор над точкой начала
            this->setCursor(QCursor(Qt::CursorShape::CrossCursor));
            if(leftPress)
            {
                QPointF p = event->scenePos() - event->lastScenePos();
                lastVector = vector;
                vector.begin   += p;
                emit vectorChanged(vector);
                scene()->update();
                lastHandler = BeginPoint;
            }
        }
        else if(this->point2Border(checker).contains(event->scenePos()) &&
                (lastHandler == Nothing || lastHandler == EndPoint))
        {
            this->setCursor(QCursor(Qt::CursorShape::CrossCursor));
            // Курсор над точкой конца вектора
            if(leftPress)
            {
                QPointF p = event->scenePos() - event->lastScenePos();
                lastVector = vector;
                vector.end   += p;
                emit vectorChanged(vector);
                scene()->update();
                lastHandler = EndPoint;
            }
        }
        else if(this->lineBorder(checker).contains(event->scenePos()) &&
                (lastHandler == Nothing || lastHandler == Line))
        {
            this->setCursor(QCursor(Qt::CursorShape::SizeAllCursor));
            // Курсор над линией
            if(leftPress)
            {
                QPointF p = event->scenePos() - event->lastScenePos();
                lastVector = vector;
                vector.begin += p;
                vector.end   += p;
                emit vectorChanged(vector);
                scene()->update();
                lastHandler = Line;
            }
        }
        else
        {
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

// --------------------------- hoverLeaveEvent --------------------------------------
void VectorObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    lastHandler = Nothing;
}

// --------------------------- shape --------------------------------------
QPainterPath VectorObject::shape() const
{
    QPainterPath path;
    path.addPath(point1Border(leftPress));
    path.addPath(point2Border(leftPress));
    path.addPath(lineBorder(leftPress));
    path.setFillRule(Qt::FillRule::WindingFill);
    return path.simplified();
}

// ======================================= VectorView =================================
VectorView::VectorView(QWidget *parent) : QGraphicsView(parent) {}

// --------------------------- setImage --------------------------------------
void VectorView::setImage(const QImage &img)
{
    backImg = img;
    QGraphicsView::update();
}

// --------------------------- drawBackground --------------------------------------
void VectorView::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (backImg.isNull())
    {
        QGraphicsView::drawBackground( painter, rect );
    }
    else
    {
        auto scRect = QRectF();
        scRect.setTopLeft(mapFromScene(rect.topLeft()));
        scRect.setBottomRight(mapFromScene(rect.bottomRight()));
        painter->save();
        painter->drawImage(rect, backImg.copy(scRect.toRect()));
        painter->restore();
    }
    QGraphicsView::drawBackground(painter, rect);
}

// =============================== VectorWidget ===========================================
VectorWidget::VectorWidget(QWidget *parent) : QMainWindow(parent)
{
    // Создаем view и настраиваем его в layout
    view = new VectorView(this);
    setCentralWidget(view);
    view->setMouseTracking(true);
    setMouseTracking(true);

    // Добавляем сцену
    scene = new VectorScene(view);
    scene->setSceneRect(view->rect());
    view->setScene(scene);
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    scene->installEventFilter(this);

    // Добавляем строку состояния
    statLSize   = new QLabel(this);
    statLPos    = new QLabel(this);
    statBColor  = new QPushButton("Draw",this);
    this->setStatusBar( new QStatusBar(this));

    connect(statBColor, SIGNAL(clicked()),
            this, SLOT(statB_clicked()) );

    qRegisterMetaType<Vector>("Vector");

    connect(scene, SIGNAL(vectorPainted(Vector const&)),
            this, SLOT(vectorPainted_tr(Vector const&)) );

    connect(scene, SIGNAL(vectorChanged(Vector const&)),
            this, SLOT(vectorChanged_tr(Vector const&)) );

    connect(scene, SIGNAL(vectorRemove(QColor)),
            this, SIGNAL(vectorRemove(QColor)) );

    statusBar()->addWidget(statLSize);
    statusBar()->addWidget(statLPos);
    statusBar()->addWidget(statBColor);
}

// --------------------------------- start ------------------------------------
void VectorWidget::start(const QImage &image, QString const& fname)
{
    this->setFixedSize(image.width() + 2,
                       image.height() + statusBar()->height() - 2 + 2);
    statLSize->setText(
                "Size: " +
                QString::number(image.size().width()) +
                " x " +
                QString::number(image.size().height())
                );
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    view->setImage(image);
    this->setWindowTitle(fname);
    emit created(this);
}

// --------------------------------- statB_clicked() ------------------------------------
void VectorWidget::statB_clicked()
{
    static auto mode = EditMode::Drawing;
    QString newText;
    if(mode == EditMode::Drawing)
    {
        newText = "Edit";
        mode = EditMode::Editing;
    }
    else
    {
        newText = "Draw";
        mode = EditMode::Drawing;
    }
    if(scene->setEditMode(mode))
    {
        statBColor->setText(newText);
    }
}

// --------------------------------- vectorPainted_tr ------------------------------------
void VectorWidget::vectorPainted_tr(const Vector &v)
{
    static Vector temp;
    temp.begin = view->mapFromScene(v.begin);
    temp.end = view->mapFromScene(v.end);
    temp.color = v.color;
    temp.id = v.id;
    emit vectorPainted(temp);
}

// --------------------------------- vectorChanged_tr ------------------------------------
void VectorWidget::vectorChanged_tr(const Vector &v)
{
    static Vector temp;
    auto rect = view->rect();
    rect.setSize(rect.size() - QSize(2, 2));
    temp.begin = view->mapFromScene(v.begin);
    temp.end = view->mapFromScene(v.end);
    temp.color = v.color;
    temp.id = v.id;
    if(rect.contains(temp.begin.toPoint()) && rect.contains(temp.end.toPoint()))
    {
        emit vectorChanged(temp);
    }
    else
    {
        scene->stopEdit();
    }
}

// --------------------------------- updateParam ------------------------------------
void VectorWidget::updateParam(IParamData *data)
{
    auto temp1 = dynamic_cast<ParamVectorColor*>(data);
    if(temp1)
    {
        scene->setParam(temp1);
        return;
    }
    auto temp2 = dynamic_cast<ParamVectorObject*>(data);
    if(temp2)
    {
        scene->setParam(temp2);
        return;
    }
}

// --------------------------------- closeEvent ------------------------------------
void VectorWidget::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    emit closed();
}

// ---------------------------------- eventFilter ----------------------------------
bool VectorWidget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched != scene)
    {
        return false;
    }
    if(event->type() == QEvent::Type::GraphicsSceneMouseMove)
    {
        QPoint p = view->mapFromScene(
                    dynamic_cast<QGraphicsSceneMouseEvent*>(event)->scenePos());
        statLPos->setText(
                    "X: " +
                    QString::number(p.x()) +
                    " Y: " +
                    QString::number(p.y())
                    );
    }


    if(event->type() == QEvent::Type::GraphicsSceneMousePress)
    {
        auto e = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
        if(e->button() == Qt::MouseButton::MidButton)
        {
            statB_clicked();
            return true;
        }
    }

    return false;
}

// ==================================== FabricVectorEditor ==================================
IVectorEditor* FactoryVectorEditor::create(QObject* parent, IParametres* par)
{
    return new VectorEditor(parent, par);
}
