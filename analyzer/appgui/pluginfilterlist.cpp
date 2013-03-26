#include "pluginfilterlist.h"
#include "events/eventnames.h"
#include "drawengine/abstractfilter.h"
#include "io/analyzermsgsender.h"
#include "appgui/pluginfilteritem.h"

PluginFilterList::PluginFilterList(QWidget *parent) :
    QListWidget(parent)
{
    setModualName("plugin_filter_list");
    listenToEvtByName(g_strPluginFilterLoaded);
    listenToEvtByName(g_strPluginFilterUnloaded);
}

bool PluginFilterList::detonate( GitlEvent cEvt )
{
    QString& strEvtName = cEvt.getEvtName();

    if( strEvtName == g_strPluginFilterLoaded )     ///< filter loaded
    {

        QVariant vValue;
        cEvt.getEvtData().getParameter("filter", vValue);
        AbstractFilter* pFilter = (AbstractFilter*)vValue.value<void*>();
        /// make checkable & init status
        QListWidgetItem* pcItem = new QListWidgetItem();
        this->addItem(pcItem);
        PluginFilterItem *pItemWidget = new PluginFilterItem(pFilter);
        pcItem->setSizeHint(pItemWidget->sizeHint());
        setItemWidget(pcItem, pItemWidget);
    }
    else if( strEvtName == g_strPluginFilterUnloaded )  ///< filter unloaded
    {
        QVariant vValue;
        cEvt.getEvtData().getParameter("filter_name", vValue);

        /// find and remove
        QString strFilterName = vValue.toString();
        QList<QListWidgetItem*> apcFound = this->findItems(strFilterName,Qt::MatchFixedString|Qt::MatchCaseSensitive);

        for(int i = 0; i < apcFound.size(); i++)
        {
            QListWidgetItem* pcItem = apcFound.at(i);
            delete this->takeItem(row(pcItem));
        }

    }
    return true;
}
