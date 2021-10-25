#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

#include "Themeable.h"

namespace Layers
{
	class MenuBar : public QMenuBar, public Themeable
	{
		Q_OBJECT

	public:
		MenuBar(QWidget* parent = 0);

		// TODO: Make override other overloaded versions from the parent class
		QMenu* addMenu(const QString& title);

		void issue_update();

		void update_theme_dependencies();

	protected:
		QString build_stylesheet();

		void init_attributes();

		//void paintEvent(QPaintEvent* event);

	private:
		QList<QMenu*> m_menus{ QList<QMenu*>() };
	};
}

#endif // MENUBAR_H