#include "geographicsrectitem.h"
#include "../qtviewer_planetosm/osmtiles/viewer_interface.h"
#include <assert.h>
#include <math.h>
#include <QGraphicsSceneMouseEvent>
#include <QMessageBox>
#include "geographicsscene.h"
#include "qtvplugin_geomarker.h"
namespace QTVP_GEOMARKER{
	geoGraphicsRectItem::geoGraphicsRectItem(
			QString name,
			QTVOSM::viewer_interface * pVi,
			qreal cent_lat,
			qreal cent_lon,
			qreal pxwidth,
			qreal pxheight)
		:QGraphicsRectItem(0)
		,geoItemBase(name,ITEAMTYPE_RECT_POINT,pVi)
		,m_lat(cent_lat)
		,m_lon(cent_lon)
		,m_width(pxwidth)
		,m_height(pxheight)

	{
		assert(vi()!=0);
		double px,py;
		vi()->CV_LLA2World(m_lat,m_lon,&px,&py);
		setRect(px - pxwidth/2, py - pxheight/2, pxwidth, pxheight);
	}
	void geoGraphicsRectItem::adjust_coords(int ncurrLevel)
	{
		if (vi() && ncurrLevel != level())
		{
			double ratio = pow(2.0,(ncurrLevel - level()));
			QRectF rect = this->rect();
			QPointF center = rect.center();
			setRect(center.x() * ratio - rect.width()/2,
					center.y() * ratio - rect.height()/2,
					rect.width(),
					rect.height());
		}
	}

	void geoGraphicsRectItem::setSize(qreal pxwidth,qreal pxheight)
	{
		double px,py;
		vi()->CV_LLA2World(m_lat,m_lon,&px,&py);
		m_width = pxwidth;
		m_height = pxheight;
		setRect(px - pxwidth/2, py - pxheight/2, pxwidth, pxheight);
	}

	void geoGraphicsRectItem::setCenter(qreal cent_lat,qreal cent_lon)
	{
		m_lat = cent_lat;
		m_lon = cent_lon;
		double px,py;
		vi()->CV_LLA2World(cent_lat,cent_lon,&px,&py);
		setRect(px - m_width/2, py - m_height/2, m_width, m_height);
		adjustLabelPos();
	}
	void geoGraphicsRectItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
	{
		QGraphicsRectItem::mousePressEvent(event);
		bool bshow = this->props_visible();
		this->show_props(!bshow);
		//post enent
		QMap<QString, QVariant > map_evt;
		geoGraphicsScene * pscene = dynamic_cast<geoGraphicsScene *>(this->scene());
		if (pscene)
		{
			QObject * pPlg = pscene->parent();
			if (pPlg)
			{
				qtvplugin_geomarker * pMarker = dynamic_cast<qtvplugin_geomarker *>(pPlg) ;
				if (pMarker)
				{
					map_evt["source"] = pMarker->get_name();
					map_evt["destin"] = "ALL";
					if (event->buttons() & Qt::LeftButton)
						map_evt["name"] = "ITEM_LBUTTON_CLICKED";
					else if (event->buttons() & Qt::RightButton)
						map_evt["name"] = "ITEM_RBUTTON_CLICKED";
					else if (event->buttons() & Qt::MidButton)
						map_evt["name"] = "ITEM_MBUTTON_CLICKED";
					else
						map_evt["name"] = "ITEM_BUTTON_CLICKED";
					map_evt["id"] = this->item_name();
					vi()->post_event(map_evt);
				}
			}

		}
	}

	void geoGraphicsRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
	{
		QGraphicsRectItem::mouseDoubleClickEvent(event);
		//post enent
		QMap<QString, QVariant > map_evt;
		geoGraphicsScene * pscene = dynamic_cast<geoGraphicsScene *>(this->scene());
		if (pscene)
		{
			QObject * pPlg = pscene->parent();
			if (pPlg)
			{
				qtvplugin_geomarker * pMarker = dynamic_cast<qtvplugin_geomarker *>(pPlg) ;
				if (pMarker)
				{
					map_evt["source"] = pMarker->get_name();
					map_evt["destin"] = "ALL";
					if (event->buttons() & Qt::LeftButton)
						map_evt["name"] = "ITEM_LBUTTON_DBLCLICKED";
					else if (event->buttons() & Qt::RightButton)
						map_evt["name"] = "ITEM_RBUTTON_DBLCLICKED";
					else if (event->buttons() & Qt::MidButton)
						map_evt["name"] = "ITEM_MBUTTON_DBLCLICKED";
					else
						map_evt["name"] = "ITEM_BUTTON_DBLCLICKED";
					map_evt["id"] = this->item_name();
					vi()->post_event(map_evt);
				}
			}

		}
	}



	QPointF geoGraphicsRectItem::label_pos()
	{
		return QPointF(this->rect().right(),rect().top()-8);
	}
}
