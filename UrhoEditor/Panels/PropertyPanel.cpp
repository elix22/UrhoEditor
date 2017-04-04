#include "PropertyPanel.h"

#include "../QtHelpers.h"
#include "../UrhoEditor.h"

#include <Urho3D/Scene/Serializable.h>

#include <qpushbutton.h>
#include <QApplication.h>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qcolordialog.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qtextedit.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qslider.h>
#include <qformlayout.h>
#include <qitemdelegate.h>

using namespace SprueEngine;

namespace SprueEditor
{

PropertyPanel::PropertyPanel(std::vector< std::pair<unsigned, BasePropertyPage*> >& pages, TYPE_ID_OBJECT_TO_HASH hashTranslator) :
    editing_(0x0),
    hashTranslator_(hashTranslator)
{
    propertyPages_ = new QStackedLayout(this);
    propertyPages_->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QWidget* junk = new QWidget();
    QVBoxLayout* sub = new QVBoxLayout(junk);
    QLabel* lbl = new QLabel(tr("There are no editable objects selected. Select an object in the scene, bone, or texture lists"));
    lbl->setWordWrap(true);
    sub->addWidget(lbl, 0, Qt::AlignCenter | Qt::AlignTop);
    propertyPages_->addWidget(junk);

    junk = new QWidget();
    sub = new QVBoxLayout(junk);
    lbl = new QLabel(tr("Selection has no editable properties"));
    lbl->setWordWrap(true);
    sub->addWidget(lbl, 0, Qt::AlignCenter | Qt::AlignTop);
    propertyPages_->addWidget(junk);

    int curPage = 2;
    for (auto page : pages)
    {
        typeLayouts_[page.first] = curPage;
        typeEditors_[page.first] = page.second;
        propertyPages_->addWidget(page.second);
        ++curPage;
    }

    propertyPages_->setCurrentIndex(0);
}

PropertyPanel::~PropertyPanel()
{

}

void PropertyPanel::SetEditable(std::shared_ptr<DataSource> editable)
{
    editing_ = editable;
    if (!hidden_)
        UpdateControls();
}

void PropertyPanel::showEvent(QShowEvent* event) 
{
    ScrollAreaWidget::showEvent(event); 
    hidden_ = false; 

    UpdateControls();
}

void PropertyPanel::hideEvent(QHideEvent* event) 
{ 
    ScrollAreaWidget::hideEvent(event); 
    hidden_ = true; 
}

void PropertyPanel::Link(Selectron* sel)
{
    for (auto page : typeEditors_)
        page.second->SetSelectron(sel);

    connect(sel, &Selectron::SelectionChanged, [=](void* src, Selectron* sel) {
        if (src == this)
            return;

        if (auto firstSel = sel->MostRecentSelected())
        {
            if (hashTranslator_(firstSel) != 0)
            {
                SetEditable(firstSel);
                return;
            }
        }
        SetEditable(0x0);
    });

    connect(sel, &Selectron::DataChanged, [=](void* src, Selectron* sel, unsigned hash) {
        if (src == this)
            return;
            
        if (auto firstSel = sel->MostRecentSelected())
        {
            if (hashTranslator_(firstSel) != 0)
            {
                if (firstSel == editing_)
                {
                    unsigned typeHash = hashTranslator_(editing_);
                    auto foundPage = typeLayouts_.find(typeHash);
                    auto foundProps = typeEditors_.find(typeHash);

                    if (foundPage != typeLayouts_.end() || foundProps != typeEditors_.end())
                        foundProps->second->SelectronDataChanged(src, sel, hash);
                }
            }
        }
    });
}

void PropertyPanel::UpdateControls()
{
    blockSignals(true);
    setUpdatesEnabled(false);

    if (editing_)
    {
        unsigned typeHash = hashTranslator_(editing_);
        auto foundPage = typeLayouts_.find(typeHash);
        auto foundProps = typeEditors_.find(typeHash);

        if (foundPage == typeLayouts_.end() || foundProps == typeEditors_.end())
        {
            propertyPages_->setCurrentIndex(1);
        }
        else
        {
            auto page = propertyPages_->widget(foundPage->second);
            page->blockSignals(true);
            page->setUpdatesEnabled(false);
            foundProps->second->SetEditing(editing_);
            propertyPages_->setCurrentIndex(foundPage->second);
            page->blockSignals(false);
            page->setUpdatesEnabled(true);
        }
    }
    else
    {
        propertyPages_->setCurrentIndex(0);
    }
    blockSignals(false);
    setUpdatesEnabled(true);
}

}