#ifndef CUSTOMIZEMENU_H
#define CUSTOMIZEMENU_H

#include "Button.h"
#include "Graphic.h"
#include "Layouts.h"
#include "Menu.h"
#include "ScrollArea.h"

namespace Layers
{
	class CustomizeMenu : public Menu
	{
		Q_OBJECT

	public:
		CustomizeMenu(QWidget* parent = nullptr);

		Button* apply_button() const;

		QList<CustomizePanel*>& customize_panels();

		void init_preview_window();

		void open_customize_panel(CustomizePanel* customize_panel);

		void populate_panel_layout();

		Widget* preview_widget() const;

		int calculated_topbar_content_width();

		void set_preview_widget(Widget* widget);

		int topbar_content_width(bool include_collapse_button);

	protected:
		bool eventFilter(QObject* object, QEvent* event) override;

		void init_attribute_widgets();
		void init_child_themeable_reference_list();

	private:
		void adjust_collapsed_widget();
		void collapse_text_buttons();
		void expand_text_buttons();
		void setup_layout();

		HorizontalLayout* m_main_layout{ new HorizontalLayout };
		QVBoxLayout* m_collapsed_text_buttons_layout{ new QVBoxLayout };
		QVBoxLayout* m_sidebar_layout{ new QVBoxLayout };
		QHBoxLayout* m_topbar_layout{ new QHBoxLayout };
		QGridLayout* m_preview_layout{ new QGridLayout };

		Widget* m_sidebar{ new Widget };
		Widget* m_topbar{ new Widget };
		Widget* m_preview_frame = new Widget;

		ScrollArea* m_sidebar_scroll_area{ new ScrollArea };
		ScrollArea* m_preview_scroll_area{ new ScrollArea };

		Widget* m_preview_widget{ nullptr };

		QList<CustomizePanel*> m_customize_panels;
		QList<CustomizePanel*> m_panel_stack;
		QList<Button*> m_text_button_stack;
		QList<Button*> m_topbar_text_buttons;
		QList<Button*> m_collapsed_text_buttons;
		QList<Graphic*> m_arrow_graphics;

		QSize* m_previous_size{ nullptr };

		Button* m_apply_button{ new Button("Apply", true) };
		Button* m_collapsed_button{ new Button(new Graphic(":/svgs/ellipsis.svg", QSize(32, 8)), true) };

		Graphic* m_control_arrow_graphic{ new Graphic(":/svgs/collapse_arrow_right.svg", QSize(8, 12)) };

		CustomizePanel* m_control_customize_panel{ nullptr };

		Button* m_control_text_button{ new Button("") };

		Widget* m_collapsed_widget{ new Widget };
	};
}

#endif // CUSTOMIZEMENU_H
