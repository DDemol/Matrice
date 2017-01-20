#include "OneDollarRecognizerObserver.h"

#undef min
#undef max

#define MAX_DOUBLE std::numeric_limits<double>::max();

OneDollarRecognizerObserver::OneDollarRecognizerObserver(string name) : Observer(name)
{
 //   _run = false;

    numPointsInGesture = 128;
    squareSize = 250;
    halfDiagonal = 0.5 * sqrt((250.0 * 250.0) + (250.0 * 250.0));
    anglePrecision = 2.0;
    goldenRatio = 0.5 * (-1.0 + sqrt(5.0));
    setRotationInvariance(false);
    loadTemplates();
}

OneDollarRecognizerObserver::~OneDollarRecognizerObserver()
{

}

void OneDollarRecognizerObserver::loadTemplates()
{
    SampleGestures samples;

    addTemplate(Action::CIRCLE, samples.getGestureCircle());
    addTemplate(Action::CIRCLE,samples.getGestureCircleInv());
    addTemplate(Action::X,samples.getGestureX());
    addTemplate(Action::X,samples.getGestureXInv());
    addTemplate(Action::STROKE, samples.getGestureStroke());
    addTemplate(Action::STROKE, samples.getGesturePerfectStroke());

}

int OneDollarRecognizerObserver::addTemplate(Action action, Path2D points)
{
    points = normalizePath(points);
    _templates.push_back(GestureTemplate(action, points));
    int numInstancesOfGesture = 0;

    return numInstancesOfGesture;
}

void OneDollarRecognizerObserver::setScene(GraphicsScene* scene)
{
    _scene = scene;
    QObject::connect(this,SIGNAL(changeAction(Action)),_scene,SLOT(setCurrentAction(Action)));
}

Node* OneDollarRecognizerObserver::clone(string cloneName) const
{
    return new OneDollarRecognizerObserver(cloneName);
}

bool OneDollarRecognizerObserver::start()
{
    if(!_environment)
    {
        if(_environment->getVerboseLevel() != LG_ENV_VERBOSE_MUTE)
            cout << "OneDollarRecognizerObserver Error : Parent environment has not be set." << endl;
        return false;
    }

    if(_environment->getVerboseLevel() != LG_ENV_VERBOSE_MUTE)
        cout << "Start OneDollarRecognizerObserver" << endl;

    return true;
}

bool OneDollarRecognizerObserver::stop()
{
    if(_environment->getVerboseLevel() != LG_ENV_VERBOSE_MUTE)
        cout << "Stop OneDollarRecognizerObserver" << endl;
    return true;
}

bool OneDollarRecognizerObserver::observe(map<string, Group3D *> groups3D, map<string, Group2D *> groups2D, map<string, Group1D *> groups1D, map<string, GroupSwitch *> groupsSwitch)
{

    for(map<string,Group2D*>::iterator mit = groups2D.begin();mit != groups2D.end();mit++)
    {
        set<HOrientedPoint2D*> hop = mit->second->getElementsByType("LG_ORIENTEDPOINT2D_TOUCH");
        set<HOrientedPoint2D*>::iterator itH = hop.begin();

        HOrientedPoint2D* thp = *itH;
        OrientedPoint2D* tp = thp->getLast();

        float x = tp->getPosition().getX();
        float y = tp->getPosition().getY();

        if((x != -1) && (y != -1))
        {
/*            if(! _run)
            {
                _run = true;
                _start = chrono::system_clock::now();
            }
*/
            if (_path.size() > 0 )
            {
                if(_path.back().getX() != x && _path.back().getX() != y)
                {
                    _path.push_back(Point2D (x,y));
                }
            } else
                _path.push_back(Point2D (x,y));
        }
        else if(_path.size() != 0)
        {
/*            _end = chrono::system_clock::now();
            chrono::duration<double> duree = _end-_start;

            cout << "Duree : " << duree.count() << "s" << endl;

            if(duree.count() < 1)
            {*/
                RecognitionResult rr = recognize(_path);
                emit changeAction(rr.action);
/*            }
            else
                emit changeAction(Action::DRAW);

            _run = false;*/
            _path.clear();
        }
    }

    return true;
}

set<string> OneDollarRecognizerObserver::need() const
{
    set<string> needed;
    needed.insert(LG_ORIENTEDPOINT2D_TOUCH);
    return needed;

}

RecognitionResult OneDollarRecognizerObserver::recognize(Path2D points)
{
    if (_templates.empty())
    {
        std::cout << "No templates loaded so no symbols to match." << std::endl;
        return RecognitionResult(Action::NONE, NULL);
    }

    points = normalizePath(points);
    double bestDistance = MAX_DOUBLE;
    int indexOfBestMatch = -1;

    for (int i = 0; i < (int)_templates.size(); i++)
    {
        double distance = distanceAtBestAngle(points, _templates[i]);
        if (distance < bestDistance)
        {
            bestDistance     = distance;
            indexOfBestMatch = i;
        }
    }

    double score = 1.0 - (bestDistance / halfDiagonal);

    if (-1 == indexOfBestMatch)
        return RecognitionResult(Action::NONE, 1);


    RecognitionResult bestMatch(_templates[indexOfBestMatch].action, score);
    return bestMatch;
}


Path2D OneDollarRecognizerObserver::normalizePath(Path2D points)
{
    points = resample(points);

    if (getRotationInvariance())
        points = rotateToZero(points);

    points = scaleToSquare(points);
    points = translateToOrigin(points);

    return points;
}

Path2D OneDollarRecognizerObserver::resample(Path2D points)
{
    double interval = pathLength(points) / (numPointsInGesture - 1); // interval length
    double D = 0.0;
    Path2D newPoints;

    //--- Store first point since we'll never resample it out of existence
    newPoints.push_back(points.front());

    for(int i = 1; i < (int)points.size(); i++)
    {
        Point2D currentPoint  = points[i];
        Point2D previousPoint = points[i-1];

        double d = getDistance(previousPoint, currentPoint);
        if ((D + d) >= interval)
        {
            double qx = previousPoint.getX() + ((interval - D) / d) * (currentPoint.getX() - previousPoint.getX());
            double qy = previousPoint.getY() + ((interval - D) / d) * (currentPoint.getY() - previousPoint.getY());
            Point2D point(qx, qy);

            newPoints.push_back(point);
            points.insert(points.begin() + i, point);
            D = 0.0;
        }
        else D += d;
    }

    // somtimes we fall a rounding-error short of adding the last point, so add it if so
    if (newPoints.size() == (numPointsInGesture - 1))
    {
        newPoints.push_back(points.back());
    }

    return newPoints;
}

double OneDollarRecognizerObserver::distanceAtBestAngle(Path2D points, GestureTemplate aTemplate)
{
    double startRange = -angleRange;
    double endRange   =  angleRange;


    double x1 = goldenRatio * startRange + (1.0 - goldenRatio) * endRange;
    double f1 = distanceAtAngle(points, aTemplate, x1);
    double x2 = (1.0 - goldenRatio) * startRange + goldenRatio * endRange;
    double f2 = distanceAtAngle(points, aTemplate, x2);
    while (abs(endRange - startRange) > anglePrecision)
    {

        if (f1 < f2)
        {
            endRange = x2;
            x2 = x1;
            f2 = f1;
            x1 = goldenRatio * startRange + (1.0 - goldenRatio) * endRange;
            f1 = distanceAtAngle(points, aTemplate, x1);
        }
        else
        {
            startRange = x1;
            x1 = x2;
            f1 = f2;
            x2 = (1.0 - goldenRatio) * startRange + goldenRatio * endRange;
            f2 = distanceAtAngle(points, aTemplate, x2);
        }
    }
    return min(f1, f2);
}

Path2D OneDollarRecognizerObserver::rotateToZero(Path2D points)
{
    Point2D c = centroid(points);
    double rotation = atan2(c.getY() - points[0].getY(), c.getX() - points[0].getX());
    return rotateBy(points, -rotation);
}

Path2D OneDollarRecognizerObserver::scaleToSquare(Path2D points)
{
    MyRectangle box = boundingBox(points);
    Path2D newPoints;
    for (Path2DIterator i = points.begin(); i != points.end(); i++)
    {
        Point2D point = *i;

        double scaledX = point.getX() * (this->squareSize / box.width);
        double scaledY = point.getY() * (this->squareSize / box.height);

        newPoints.push_back(Point2D(scaledX, scaledY));
    }
    return newPoints;
}

Path2D OneDollarRecognizerObserver::translateToOrigin(Path2D points)
{
    Point2D c = centroid(points);
    Path2D newPoints;
    for (Path2DIterator i = points.begin(); i != points.end(); i++)
    {
        Point2D point = *i;
        double qx = point.getX() - c.getX();
        double qy = point.getY() - c.getY();
        newPoints.push_back(Point2D(qx, qy));
    }
    return newPoints;
}

double OneDollarRecognizerObserver::pathLength(Path2D points)
{
    double distance = 0;
    for (int i = 1; i < (int)points.size(); i++)
        distance += getDistance(points[i - 1], points[i]);
    return distance;
}

double OneDollarRecognizerObserver::getDistance(Point2D p1, Point2D p2)
{
    double dx = p2.getX() - p1.getX();
    double dy = p2.getY() - p1.getY();
    double distance = sqrt((dx * dx) + (dy * dy));
    return distance;
}

double OneDollarRecognizerObserver::distanceAtAngle(
        Path2D points, GestureTemplate aTemplate, double rotation)
{
    Path2D newPoints = rotateBy(points, rotation);
    return pathDistance(newPoints, aTemplate.points);
}

Point2D OneDollarRecognizerObserver::centroid(Path2D points)
{
    double x = 0.0, y = 0.0;
    for (Path2DIterator i = points.begin(); i != points.end(); i++)
    {
        Point2D point = *i;
        x += point.getX();
        y += point.getY();
    }
    x /= points.size();
    y /= points.size();
    return Point2D(x, y);
}

Path2D OneDollarRecognizerObserver::rotateBy(Path2D points, double rotation)
{
    Point2D c     = centroid(points);
    double cosine = cos(rotation);
    double sine   = sin(rotation);

    Path2D newPoints;
    for (Path2DIterator i = points.begin(); i != points.end(); i++)
    {
        Point2D point = *i;
        double qx = (point.getX() - c.getX()) * cosine - (point.getY() - c.getY()) * sine   + c.getX();
        double qy = (point.getX() - c.getX()) * sine   + (point.getY() - c.getY()) * cosine + c.getY();
        newPoints.push_back(Point2D(qx, qy));
    }
    return newPoints;
}

double OneDollarRecognizerObserver::pathDistance(Path2D pts1, Path2D pts2)
{
    // assumes pts1.size == pts2.size

    double distance = 0.0;
    for (int i = 0; i < (int)pts1.size(); i++)
        distance += getDistance(pts1[i], pts2[i]);
    return (distance / pts1.size());
}

MyRectangle OneDollarRecognizerObserver::boundingBox(Path2D points)
{
    double minX =  MAX_DOUBLE;
    double maxX = -MAX_DOUBLE;
    double minY =  MAX_DOUBLE;
    double maxY = -MAX_DOUBLE;

    for (Path2DIterator i = points.begin(); i != points.end(); i++)
    {
        Point2D point = *i;
        if (point.getX() < minX)
            minX = point.getX();
        if (point.getX() > maxX)
            maxX = point.getX();
        if (point.getY() < minY)
            minY = point.getY();
        if (point.getY() > maxY)
            maxY = point.getY();
    }
    MyRectangle bounds(minX, minY, (maxX - minX), (maxY - minY));
    return bounds;
}

void   OneDollarRecognizerObserver::setRotationInvariance(bool ignoreRotation)
{
    if (ignoreRotation)
        angleRange = 45.0;
    else
        angleRange = 15.0;
}
