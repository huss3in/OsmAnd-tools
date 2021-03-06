#include "MapLayersData.h"
#include "Utilities.h"

MapLayersData::MapLayersData(QObject *) : app(OsmAnd::OsmAndApplication::getAndInitializeApplication()),
    lastRenderedImage(nullptr)
{

}

QImage* MapLayersData::getRenderedImage(OsmAnd::AreaI* bbox) {
    if(lastRenderedImage == nullptr) {
        return lastRenderedImage;
    }
    *bbox = this->lastRenderedBox;
    return lastRenderedImage;
}

void MapLayersData::setRenderedImage(SkBitmap& bmp, OsmAnd::AreaI bbox) {
    if(lastRenderedImage != nullptr) {
        delete lastRenderedImage;
    }
    lastRenderedImage = new QImage(bmp.width(), bmp.height(), QImage::Format_ARGB32);
    void* pixels = bmp.getPixels();
    memcpy(lastRenderedImage->bits(), pixels, bmp.getSize());
    lastRenderedBox = bbox;
}

void MapLayersData::setRoute(QList< std::shared_ptr<OsmAnd::RouteSegment> >& r)
{
    this->route.clear();
    for(std::shared_ptr<OsmAnd::RouteSegment> rt : r) {
        int k = rt->startPointIndex;
        while(k != rt->endPointIndex) {
            this->route.append(OsmAnd::PointF(OsmAnd::Utilities::get31LongitudeX(rt->road->points[k].x),
                                              OsmAnd::Utilities::get31LatitudeY(rt->road->points[k].y)));
            if(k == rt->startPointIndex) {
                this->routeText.append(rt->description);
            } else {
                this->routeText.append("");
            }
            k = k > rt->endPointIndex? k - 1 : k + 1;
        }
    }
}

void MapLayersData::setMapLatLonZoom(double lat,double lon,int zoom) {
    app->getSettings()->MAP_SHOW_LATITUDE.set(QVariant(lat));
    app->getSettings()->MAP_SHOW_LONGITUDE.set(QVariant(lon));
    app->getSettings()->MAP_SHOW_ZOOM.set(QVariant(zoom));
}

void MapLayersData::setTargetLatLon(double lat,double lon) {
    app->getSettings()->TARGET_LATITUDE.set(QVariant(lat));
    app->getSettings()->TARGET_LONGITUDE.set(QVariant(lon));
}

void MapLayersData::setStartLatLon(double lat,double lon) {
    app->getSettings()->START_LATITUDE.set(QVariant(lat));
    app->getSettings()->START_LONGITUDE.set(QVariant(lon));
}

double MapLayersData::getMapLatitude() {
    return app->getSettings()->MAP_SHOW_LATITUDE.get().toDouble();
}

double MapLayersData::getMapLongitude() {
    return app->getSettings()->MAP_SHOW_LONGITUDE.get().toDouble();
}

int MapLayersData::getMapZoom() {
    return app->getSettings()->MAP_SHOW_ZOOM.get().toInt();
}

bool MapLayersData::isTargetPresent() {
    return app->getSettings()->TARGET_LATITUDE.present();
}

double MapLayersData::getTargetLatitude() {
    return app->getSettings()->TARGET_LATITUDE.get().toDouble();
}

double MapLayersData::getTargetLongitude() {
    return app->getSettings()->TARGET_LONGITUDE.get().toDouble();
}

bool MapLayersData::isStartPresent() {
    return app->getSettings()->START_LATITUDE.present();
}

double MapLayersData::getStartLatitude() {
    return app->getSettings()->START_LATITUDE.get().toDouble();
}

double MapLayersData::getStartLongitude() {
    return app->getSettings()->START_LONGITUDE.get().toDouble();
}
