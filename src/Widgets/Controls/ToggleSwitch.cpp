#include "../../../include/Label.h"
#include "../../../include/ToggleSwitch.h"

#include <QHBoxLayout>
#include <QMouseEvent>

using Layers::ToggleSwitch;

ToggleSwitch::ToggleSwitch(bool vertical, QWidget* parent) :
	m_vertical{ vertical }, Widget(parent)
{
	init_child_themeable_reference_list();
	init_attributes();

	installEventFilter(this);
	setFixedSize(45, 45);
	set_name("toggle_switch");

	m_square->set_name("square");
	m_square->setFixedSize(11, 11);

	update_spacer_size();
	m_spacer->hide();

	setup_layout();
}

void ToggleSwitch::replace_all_attributes_with(ToggleSwitch* tswitch)
{
	Widget::replace_all_attributes_with(tswitch);

	if (m_square) m_square->replace_all_attributes_with(tswitch->m_square);
}

void ToggleSwitch::setFixedHeight(int h)
{
	Widget::setFixedHeight(h);

	update_spacer_size();
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
	}
	else
	{
		set_state("Untoggled");

		m_spacer->hide();
	}

	emit toggled_event();

	update();
}

bool ToggleSwitch::toggled() const
{
	if (state() == "Toggled") return true;
	else return false;
}

void ToggleSwitch::update_layout_margins()
{
	int b_thickness = a_border_thickness.value<int>();

	if (m_layout_v)
		m_layout_v->setContentsMargins(
			0, a_margin_top.value<int>() + b_thickness + a_padding_top.value<int>(),
			0, a_padding_bottom.value<int>() + b_thickness + a_margin_bottom.value<int>());
	else if (m_layout_h)
		m_layout_h->setContentsMargins(
			a_margin_left.value<int>() + b_thickness + a_padding_left.value<int>(), 0,
			a_padding_right.value<int>() + b_thickness + a_margin_right.value<int>(), 0);
}

void ToggleSwitch::update_spacer_size()
{
	int b_thickness = a_border_thickness.value<int>();

	if (m_vertical)
	{
		m_spacer->setFixedSize(
			0, height() - a_margin_top.value<int>() - b_thickness - a_padding_top.value<int>() - m_square->height() - a_padding_bottom.value<int>() - b_thickness - a_margin_bottom.value<int>()
		);
	}
	else
	{
		m_spacer->setFixedSize(
			width() - a_margin_left.value<int>() - b_thickness - a_padding_left.value<int>() - m_square->width() - a_padding_right.value<int>() - b_thickness - a_margin_right.value<int>(), 0
		);
	}
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
	a_border_fill.set_values({
		{ "Untoggled", QColor(Qt::black) },
		{ "Toggled", QColor("#6fc65b") }
		});
	a_border_thickness.set_value(3);
	a_corner_radius_tl.set_value(4);
	a_corner_radius_tr.set_value(4);
	a_corner_radius_bl.set_value(4);
	a_corner_radius_br.set_value(4);
	a_fill.set_values({
		{ "Untoggled", QColor(Qt::white) },
		{ "Toggled", QColor("#6fc65b") }
		});
	if (m_vertical)
	{
		a_margin_left.set_value(12);
		a_margin_right.set_value(12);
	}
	else
	{
		a_margin_top.set_value(12);
		a_margin_bottom.set_value(12);
	}

	m_square->a_corner_radius_tl.set_value(2);
	m_square->a_corner_radius_tr.set_value(2);
	m_square->a_corner_radius_bl.set_value(2);
	m_square->a_corner_radius_br.set_value(2);
	m_square->a_fill.set_values({
		{ "Untoggled", QColor(Qt::black) },
		{ "Toggled", QColor(Qt::white) }
		});

	//m_spacer->a_fill.set_disabled();
	m_spacer->a_fill.set_value(QColor(Qt::blue));

	// Set initial state
	set_state("Untoggled");

	connect(&a_margin_left, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_margin_top, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_margin_right, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_margin_bottom, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_padding_left, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_padding_top, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_padding_right, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});

	connect(&a_padding_bottom, &Attribute::value_changed, [this] {
		update_layout_margins(); update_spacer_size();
		});
}

void ToggleSwitch::init_child_themeable_reference_list()
{
	add_child_themeable_reference(m_square);
}

void ToggleSwitch::setup_layout()
{
	if (m_vertical)
	{
		m_layout_v = new QVBoxLayout;

		update_layout_margins();

		m_layout_v->setSpacing(0);
		m_layout_v->addWidget(m_spacer);
		m_layout_v->addWidget(m_square);
		m_layout_v->addStretch();
		m_layout_v->setAlignment(m_spacer, Qt::AlignHCenter);
		m_layout_v->setAlignment(m_square, Qt::AlignHCenter);

		setLayout(m_layout_v);
	}
	else
	{
		m_layout_h = new QHBoxLayout;

		update_layout_margins();

		m_layout_h->setSpacing(0);
		m_layout_h->addWidget(m_spacer);
		m_layout_h->addWidget(m_square);
		m_layout_h->addStretch();
		m_layout_h->setAlignment(m_spacer, Qt::AlignVCenter);
		m_layout_h->setAlignment(m_square, Qt::AlignVCenter);

		setLayout(m_layout_h);
	}
}