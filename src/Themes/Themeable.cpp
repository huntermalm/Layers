#include "../../../include/Attribute.h"
#include "../../../include/AttributeWidgets.h"
#include "../../../include/CustomizePanel.h"
#include "../../../include/Graphic.h"
#include "../../../include/Theme.h"
#include "../../../include/Themeable.h"

using Layers::Attribute;
using Layers::AttributeLayoutItem;
using Layers::CustomizePanel;
using Layers::Graphic;
using Layers::Themeable;
using Layers::Theme;

Themeable::~Themeable()
{
	if (m_name) delete m_name;
	if (m_proper_name) delete m_proper_name;

	m_name = nullptr;
	m_proper_name = nullptr;
}

QMap<QString, Attribute*>& Themeable::attributes()
{
	return m_attributes;
}

QList<AttributeLayoutItem*>& Themeable::attribute_layout()
{
	return m_attribute_layout;
}

CustomizePanel* Themeable::customize_panel() const
{
	return m_customize_panel;
}

CustomizePanel* Themeable::init_customize_panel()
{
	m_customize_panel = new CustomizePanel(this);

	m_customize_panel->setup_layout();
	
	return m_customize_panel;
}

Graphic* Themeable::icon() const
{
	return m_icon;
}

void Themeable::init_child_themeable_reference_list()
{
}

void Themeable::initialize_and_acquire_panels(QList<CustomizePanel*>& list)
{
	for (Themeable* child_themeable : m_child_themeable_references)
	{
		// TODO: Consider a Themeable::is_customizable() function so this is clearer
		if (child_themeable->proper_name()) child_themeable->initialize_and_acquire_panels(list);
	}

	list.append(init_customize_panel());
}

bool Themeable::is_stateful() const
{
	return m_is_stateful;
}

void Themeable::apply_theme(Theme& theme)
{
	if (m_current_theme != &theme) m_current_theme = &theme;

	if (!m_name) qDebug() << "Unable to apply theme.  You must apply a name to the widget first.";
	else
	{
		if (theme.contains_attributes_for_tag(tag()))
		{
			//if (tag() == "layers/customize_menu/sidebar/customize_panel/corner_radii_aw/line_editor")
			//{
			//	qDebug() << "Found CRAW Line Editor";
			//}

			apply_theme_attributes(theme[tag()]);

			QMap<QString, Attribute*>& theme_attrs = theme[tag()];

			//for (const QString& attr_tag : m_attributes.keys())
			//{
			//	m_attributes[attr_tag]->copy_values_from(theme_attrs[attr_tag]);
			//}

			//update_theme_dependencies();
		}

		for (Themeable* child_themeable : m_child_themeable_references)
			child_themeable->apply_theme(theme);
	}
}

void Themeable::apply_theme_attributes(QMap<QString, Attribute*>& theme_attrs)
{
}

void Themeable::reapply_theme()
{
	if (m_current_theme) apply_theme(*m_current_theme);
}

void Themeable::add_child_themeable_reference(Themeable* child_themeable)
{
	// TODO: Consider a version of this function that can also name the child themeable
	m_child_themeable_references.append(child_themeable);

	if (m_tag_prefixes_assigned)
	{
		if (*m_name == "window") child_themeable->assign_tag_prefixes(m_tag_prefixes, "");
		else child_themeable->assign_tag_prefixes(m_tag_prefixes, *m_name);
	}
}

void Themeable::remove_child_themeable_reference(Themeable* child_themeable)
{
	m_child_themeable_references.removeOne(child_themeable);

	child_themeable->unassign_prefixes();
}

//void Themeable::replace_attribute_with_proxy(const QString& attribute_name, Attribute* proxy_attribute)
//{
//	if (m_attribute_set.replace_with_proxy(attribute_name, proxy_attribute))
//	{
//		proxy_attribute->connect(proxy_attribute, &Attribute::value_changed, [this]
//			{
//				update_theme_dependencies();
//				issue_update();
//			});
//	}
//}

void Themeable::set_is_app_themeable(bool is_app_themeable)
{
	m_is_app_themeable = is_app_themeable;

	for (Themeable* m_child_themeable : m_child_themeable_references)
		m_child_themeable->set_is_app_themeable(is_app_themeable);
}

void Themeable::set_functionality_disabled(bool disabled)
{
	m_functionality_disabled = disabled;
}

void Themeable::assign_tag_prefixes(QList<QString> parent_prefixes, const QString& parent_name)
{
	if (parent_name != "")
	{
		m_tag_prefixes.append(parent_prefixes);
		m_tag_prefixes.append(parent_name);
	}

	for (Themeable* child_themeable : m_child_themeable_references)
	{
		child_themeable->assign_tag_prefixes(m_tag_prefixes, *m_name);
	}

	m_tag_prefixes_assigned = true;
}

void Themeable::unassign_prefixes()
{
	m_tag_prefixes.clear();

	m_tag_prefixes_assigned = false;

	for (Themeable* child_themeable : m_child_themeable_references)
	{
		child_themeable->unassign_prefixes();
	}
}

//QMap<QString, AttributeWidget*>& Themeable::attribute_widgets()
//{
//	return m_attribute_widgets;
//}

QList<Themeable*>& Themeable::child_themeable_references()
{
	return m_child_themeable_references;
}

//void Themeable::copy_attribute_values_to(Theme* theme)
//{
//	if (theme->contains_attributes_for_tag(tag()))
//	{
//		theme->copy_attribute_values_of(this);
//
//		for (Themeable* child_themeable : m_child_themeable_references)
//			child_themeable->copy_attribute_values_to(theme);
//	}
//}

Theme* Themeable::current_theme()
{
	return m_current_theme;
}

void Themeable::unfilter_attribute(const QString& attribute)
{
	m_filtered_attributes.removeOne(attribute);
}

void Themeable::set_icon(Graphic* icon)
{
	if (m_icon) m_icon->deleteLater();

	m_icon = icon;
}

void Themeable::set_name(const QString& name)
{
	if (m_name) delete m_name;

	m_name = new QString(name);
}

void Themeable::set_proper_name(const QString& proper_name)
{
	if (m_proper_name) delete m_proper_name;

	m_proper_name = new QString(proper_name);
}

void Themeable::set_state(const QString& state)
{
	if (m_state != state)
	{
		m_state = state;

		for (Attribute* attr : m_attributes)
			attr->set_state(state);

		for (Themeable* child_themeable : m_child_themeable_references)
			child_themeable->set_state(state);

		//update_theme_dependencies();

		//issue_update();
	}
}

QString* Layers::Themeable::proper_name() const
{
	return m_proper_name;
}

QString& Themeable::tag()
{
	if (m_tag == "")
	{
		if (m_is_app_themeable)
		{
			//m_tag += "app/" + *m_app_name + "/";
			m_tag += "app/";
		}
		else
		{
			m_tag += "layers/";
		}

		for (QString supplemental_prefix : m_tag_prefixes)
		{
			m_tag += supplemental_prefix + "/";
		}

		if (m_name) m_tag += *m_name;
	}

	return m_tag;
}

//QMap<QString, StatefulAttribute>& Themeable::stateful_attributes()
//{
//	return m_stateful_attributes;
//}

//QList<QString> Themeable::states() const
//{
//	return m_attribute_set.states();
//}

//void Themeable::update_theme_dependencies()
//{
//}

void Themeable::init_attributes()
{
}

//void Themeable::init_attribute_widgets()
//{
//}

QString Themeable::state() const
{
	return m_state;
}
