#include "../../../include/Label.h"
#include "../../../include/ToggleSwitch.h"

#include <QHBoxLayout>
#include <QMouseEvent>

using Layers::ToggleSwitch;

ToggleSwitch::ToggleSwitch(QWidget* parent) : Widget(parent)
{
	init_child_themeable_reference_list();
	init_attributes();

	installEventFilter(this);

	m_switch->set_name("switch");
	m_switch->setFixedSize(45, 21);

	m_spacer->setFixedWidth(25);
	m_spacer->hide();

	m_square->set_name("square");
	m_square->setFixedSize(11, 11);

	setFixedSize(45, 45);

	setup_layout();
}

ToggleSwitch::ToggleSwitch(const QString& first_label_text, const QString& second_label_text, QWidget* parent) :
	m_first_label{ new Label(first_label_text) },
	m_second_label{ new Label(second_label_text) },
	Widget(parent)
{
	init_child_themeable_reference_list();
	init_attributes();

	installEventFilter(this);

	m_switch->set_name("switch");
	m_switch->setFixedSize(45, 21);

	m_spacer->setFixedWidth(25);
	m_spacer->hide();

	m_square->set_name("square");
	m_square->setFixedSize(11, 11);

	m_first_label_opacity->setOpacity(1.0);
	m_second_label_opacity->setOpacity(0.5);

	m_first_label->set_name("text");
	m_first_label->set_font_size(14);
	m_first_label->set_padding(0, 10, 0, 0);
	m_first_label->setGraphicsEffect(m_first_label_opacity);

	m_second_label->set_name("text");
	m_second_label->set_font_size(14);
	m_second_label->set_padding(0, 10, 0, 0);
	m_second_label->setGraphicsEffect(m_second_label_opacity);

	setFixedSize(m_first_label->width() + 10 + m_switch->width() + 10 + m_second_label->width(), 45);

	setup_layout();
}

void ToggleSwitch::set_toggled(bool toggled)
{
	if ((toggled && state() != "Toggled") || (!toggled && state() == "Toggled"))
	{
		toggle();
	}
}

void ToggleSwitch::toggle()
{
	if (state() == "Untoggled")
	{
		set_state("Toggled");

		m_spacer->show();

		m_first_label_opacity->setOpacity(0.5);
		m_second_label_opacity->setOpacity(1.0);
	}
	else
	{
		set_state("Untoggled");

		m_spacer->hide();

		m_first_label_opacity->setOpacity(1.0);
		m_second_label_opacity->setOpacity(0.5);
	}

	emit toggled_event();
}

bool ToggleSwitch::toggled() const
{
	if (state() == "Toggled") return true;
	else return false;
}

bool ToggleSwitch::eventFilter(QObject* object, QEvent* event)
{
	// TODO: See if you want to call super's eventFilter()
	if ((event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonDblClick) && object == this)
	{
		QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);

		if (mouse_event->button() & Qt::LeftButton)
		{
			toggle();
		}
	}

	return false;
}

void ToggleSwitch::init_attributes()
{
	set_stateless_attribute_value("background_disabled", true);

	m_square->set_stateless_attribute_value("corner_radius_tl", 2);
	m_square->set_stateless_attribute_value("corner_radius_tr", 2);
	m_square->set_stateless_attribute_value("corner_radius_bl", 2);
	m_square->set_stateless_attribute_value("corner_radius_br", 2);

	m_switch->set_stateless_attribute_value("border_thickness", 3);
	m_switch->set_stateless_attribute_value("corner_radius_tl", 4);
	m_switch->set_stateless_attribute_value("corner_radius_tr", 4);
	m_switch->set_stateless_attribute_value("corner_radius_bl", 4);
	m_switch->set_stateless_attribute_value("corner_radius_br", 4);

	m_spacer->set_stateless_attribute_value("background_disabled", true);

	m_switch->convert_attribute_to_stateful("background_color", {
		{ "Untoggled", QColor(Qt::white) },
		{ "Toggled", QColor("#6fc65b") }
		});

	m_switch->convert_attribute_to_stateful("border_color", {
		{ "Untoggled", QColor(Qt::black) },
		{ "Toggled", QColor("#6fc65b") }
		});

	m_square->convert_attribute_to_stateful("background_color", {
		{ "Untoggled", QColor(Qt::black) },
		{ "Toggled", QColor("#6fc65b") }
		});

	// Set initial state
	set_state("Untoggled");
}

void ToggleSwitch::init_child_themeable_reference_list()
{
	if (m_first_label) add_child_themeable_reference(m_first_label);
	if (m_second_label) add_child_themeable_reference(m_second_label);

	m_switch->add_child_themeable_reference(m_square);
	add_child_themeable_reference(m_switch);
}

void ToggleSwitch::setup_layout()
{
	QHBoxLayout* main_layout = new QHBoxLayout;
	QHBoxLayout* switch_layout = new QHBoxLayout;

	switch_layout->setContentsMargins(5, 0, 5, 0);
	switch_layout->setSpacing(0);
	switch_layout->addWidget(m_spacer);
	switch_layout->addWidget(m_square);
	switch_layout->addStretch();

	m_switch->setLayout(switch_layout);

	main_layout->setContentsMargins(0, 0, 0, 0);
	main_layout->setSpacing(10);

	if (m_first_label && m_second_label)
	{
		main_layout->addWidget(m_first_label);
		main_layout->addWidget(m_switch);
		main_layout->addWidget(m_second_label);
	}
	else
	{
		main_layout->addWidget(m_switch);
	}

	setLayout(main_layout);
}