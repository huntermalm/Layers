#include "../../../include/Slider.h"

#include <QApplication>
#include <QMouseEvent>
#include <QVBoxLayout>

using Layers::Slider;

Slider::Slider(int limit, QWidget* parent) :
	m_limit{ limit },
	Widget(parent)
{
	init_attributes();
	init_child_themeable_reference_list();

	installEventFilter(this);
	setFixedSize(244, 45);

	m_bar->setFixedHeight(5);
	m_bar->set_name("bar");
	m_bar->set_margin(23, 0, 23, 0);

	m_handle->setFixedSize(45, 45);
	m_handle->set_name("handle");
	m_handle->set_margin(15, 5, 15, 5);

	setup_layout();
}

void Slider::init_attributes()
{
	//add_attribute("value", 0);

	//a_fill.set_disabled();

	a_corner_radius_tl.set_value(10); // Need to check these values
	a_corner_radius_tr.set_value(10);
	a_corner_radius_bl.set_value(10);
	a_corner_radius_br.set_value(10);

	m_bar->a_corner_radius_tl.set_value(2);
	m_bar->a_corner_radius_tr.set_value(2);
	m_bar->a_corner_radius_bl.set_value(2);
	m_bar->a_corner_radius_br.set_value(2);

	m_handle->a_corner_radius_tl.set_value(3);
	m_handle->a_corner_radius_tr.set_value(3);
	m_handle->a_corner_radius_bl.set_value(3);
	m_handle->a_corner_radius_br.set_value(3);

	//m_bar->set_attribute_value("corner_radius_tl", 2);
	//m_bar->set_attribute_value("corner_radius_tr", 2);
	//m_bar->set_attribute_value("corner_radius_bl", 2);
	//m_bar->set_attribute_value("corner_radius_br", 2);
	
	//m_handle->set_attribute_value("corner_radius_tl", 3);
	//m_handle->set_attribute_value("corner_radius_tr", 3);
	//m_handle->set_attribute_value("corner_radius_bl", 3);
	//m_handle->set_attribute_value("corner_radius_br", 3);
}

void Slider::init_child_themeable_reference_list()
{
	add_child_themeable_reference(m_bar);
	add_child_themeable_reference(m_handle);
}

void Slider::set_value(int value)
{
	a_value.set_value(value);
	update_handle_pos();

	emit value_changed(value);
}

void Slider::update_handle_pos()
{
	double drag_increment = double(width() - m_handle->width()) / double(m_limit);

	m_handle->move(drag_increment * a_value.value<int>(), m_handle->y());
}

void Slider::update_theme_dependencies()
{
	update_handle_pos();
}

bool Slider::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::MouseButtonPress && m_handle->underMouse())
	{
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

		if (mouse_event->button() & Qt::LeftButton)
		{
			m_dragging_handle = true;

			m_mouse_click_position = mouse_event->pos();
			m_value_on_click = a_value.value<int>();
		}
	}
	else if (event->type() == QEvent::MouseButtonRelease && m_handle->underMouse())
	{
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

		if (mouse_event->button() & Qt::LeftButton)
		{
			m_dragging_handle = false;
		}
	}

	if (event->type() == QEvent::MouseMove && m_dragging_handle == true)
	{
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

		QPoint delta = mouse_event->pos() - m_mouse_click_position;

		double drag_increment = double(m_bar->width() - m_handle->width()) / double(m_limit);

		int new_value = m_value_on_click + int(delta.x() / drag_increment);

		if (new_value < 0)
		{
			if (a_value.value<int>() != 0)
			{
				set_value(0);
			}
		}
		else if (new_value > m_limit)
		{
			if (a_value.value<int>() != m_limit)
			{
				set_value(m_limit);
			}
		}
		else
		{
			set_value(new_value);
		}
	}

	return false;
}

void Slider::setup_layout()
{
	QVBoxLayout* main_layout = new QVBoxLayout;

	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setSpacing(0);
	main_layout->addWidget(m_bar);

	setLayout(main_layout);

	m_handle->raise();
}
