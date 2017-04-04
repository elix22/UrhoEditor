#include "UrhoDataSources.h"

#include "../Commands/Urho/UrhoPropertyCmd.h"
#include "../UrhoEditor.h"
#include "../GlobalAccess.h"

#include <EditorLib/Selectron.h>

namespace UrhoUI
{

    SerializableDataSource::SerializableDataSource(Urho3D::Serializable* serial)
    {
        serial_ = serial;
    }

    void SerializableDataSource::Destroy()
    {
        serial_.Reset();
    }

    bool SerializableDataSource::equals(const DataSource* rhs) const
    {
        if (const SerializableDataSource* other = dynamic_cast<const SerializableDataSource*>(rhs))
            return other->serial_ == serial_;
        return false;
    }

    QString SerializableDataSource::GetName() const
    {
        return "<not nameable>";
    }

    QString SerializableDataSource::GetTypeName() const
    {
        if (serial_)
            return serial_->GetTypeName().CString();
        return "<invalid type - null>";
    }

    QString NodeDataSource::GetName() const
    {
        if (GetNode() && !GetNode()->GetName().Empty())
            return GetNode()->GetName().CString();
        return "<unnamed>";
    }

    QString ComponentDataSource::GetName() const
    {
        return "<not nameable>";
    }
    
    QString UIDataSource::GetName() const
    {
        if (GetElement() && !GetElement()->GetName().Empty())
            return GetElement()->GetName().CString();
        return "<unnamed>";
    }
 
    bool SerializableDataGridHandler::CanHandleDataSource(std::shared_ptr<DataSource> dataSource) const
    {
        return std::dynamic_pointer_cast<SerializableDataSource>(dataSource).get() != 0x0;
    }

    std::vector<std::string> SerializableDataGridHandler::GetColumns(std::shared_ptr<DataSource> dataSource) const
    {
        std::vector<std::string> ret;
        if (auto ds = std::dynamic_pointer_cast<SerializableDataSource>(dataSource))
        {
            if (auto editable = ds->GetObject())
            {
                auto attrInfo = editable->GetAttributes();
                if (!attrInfo)
                    return ret;

                for (auto item : *attrInfo)
                {
                    if (item.mode_ & Urho3D::AM_NOEDIT)
                        continue;
                    ret.push_back(item.name_.CString());
                }
            }
        }
        return ret;
    }

    void SerializableDataGridHandler::FromText(std::shared_ptr<DataSource> dataSource, const std::string& column, const std::string& textValue)
    {
        if (auto ds = std::dynamic_pointer_cast<SerializableDataSource>(dataSource))
        {
            if (auto editable = ds->GetObject())
            {
                auto attrs = editable->GetAttributes();
                auto oldValue = editable->GetAttribute(column.c_str());
                unsigned attrIndx = 0;
                for (unsigned i = 0; i < attrs->Size(); ++i)
                {
                    if ((*attrs)[i].name_.Compare(column.c_str(), false) == 0)
                    {
                        attrIndx = i;
                        break;
                    }
                }

                auto newValue = oldValue;
                if (oldValue.GetType() == Urho3D::VAR_QUATERNION)
                {
                    Urho3D::Quaternion quat;
                    newValue.FromString(Urho3D::VAR_VECTOR3, textValue.c_str());
                    Urho3D::Vector3 vec = newValue.GetVector3();
                    quat.FromEulerAngles(vec.x_, vec.y_, vec.z_);
                    newValue = quat;
                }
                else
                    newValue.FromString(oldValue.GetType(), textValue.c_str());
                
                UrhoUI::UrhoPropertyCmd* cmd = new UrhoUI::UrhoPropertyCmd(ds, attrIndx, newValue, oldValue);
                Global_DocumentManager()->GetActiveDocument()->GetUndoStack()->push(cmd);
                SprueEditor::UrhoEditor::GetInstance()->GetObjectSelectron()->NotifyDataChanged(this, ds);
            }
        }
    }

    QString SerializableDataGridHandler::ToText(std::shared_ptr<DataSource> dataSource, const std::string& column) const
    {
        if (auto ds = std::dynamic_pointer_cast<SerializableDataSource>(dataSource))
        {
            if (auto editable = ds->GetObject())
            {
                auto var = editable->GetAttribute(column.c_str());
                if (var.GetType() == Urho3D::VAR_QUATERNION)
                    var = var.GetQuaternion().EulerAngles();
                return QString(var.ToString().CString());
            }
        }
        return QString("<invalid>");
    }
}