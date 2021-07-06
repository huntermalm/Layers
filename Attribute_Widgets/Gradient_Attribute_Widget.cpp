#include "../Layers.h"

using Layers::Attribute;
using Layers::Gradient_Attribute_Widget;
using Layers::Theme;

Gradient_Attribute_Widget::Gradient_Attribute_Widget(const QString& attribute_label_text, Attribute& attribute, bool is_primary, QWidget* parent) :
	m_attribute_label{ new Label(attribute_label_text) }, Attribute_Widget(is_primary, parent)
{
	init_child_themeable_reference_list();

	m_attribute = &attribute;

	if (attribute.is_stateful())
		set_customize_states(attribute.states());

	// Setup Attribute Label
	m_attribute_label->set_name("label");
	m_attribute_label->set_font_size(14);
	m_attribute_label->set_padding(0, 7, 0, 0);

	// Setup Left Stretch
	m_left_stretch->set_attribute_value("Default", "background_disabled", true);
	m_left_stretch->hide();

	// Setup Right Stretch
	m_right_stretch->set_attribute_value("Default", "background_disabled", true);

	// Setup Color Control
	m_gradient_control_asc = m_gradient_control->share_attribute_with_themeable("Default", m_gradient_control->attributes()["background_gradient_stops"], attribute.state(), attribute, true); // m_customizing_state

	// Setup Layout
	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->setContentsMargins(8, 0, 8, 0);
	hbox->setSpacing(0);
	hbox->addWidget(m_left_stretch);
	hbox->addWidget(m_attribute_label);
	hbox->addSpacing(6);
	hbox->addWidget(m_gradient_control);
	hbox->addWidget(m_right_stretch);
	setLayout(hbox);
}

void Gradient_Attribute_Widget::apply_theme(Theme& theme)
{
	if (m_gradient_control_asc) m_gradient_control_asc->obtain_attribute();

	Themeable::apply_theme(theme);
}

void Gradient_Attribute_Widget::set_centered(bool centered)
{
	if (m_centered != centered)
	{
		m_centered = centered;

		if (m_centered) m_left_stretch->show();
		else m_left_stretch->hide();
	}
}

void Gradient_Attribute_Widget::init_child_themeable_reference_list()
{
	add_child_themeable_reference(m_attribute_label);
	add_child_themeable_reference(m_gradient_control);
}