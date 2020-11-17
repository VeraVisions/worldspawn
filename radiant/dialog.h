/*
   Copyright (C) 1999-2006 Id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#if !defined( INCLUDED_DIALOG_H )
#define INCLUDED_DIALOG_H

#include <list>
#include <uilib/uilib.h>
#include "property.h"

#include "generic/callback.h"
#include "gtkutil/dialog.h"
#include "generic/callback.h"
#include "string/string.h"

struct DLG_DATA {
    virtual ~DLG_DATA() = default;

    virtual void release() = 0;

    virtual void importData() const = 0;

    virtual void exportData() const = 0;
};


template<typename FirstArgument>
class CallbackDialogData;

typedef std::list<DLG_DATA *> DialogDataList;

class Dialog {
    ui::Window m_window;
    DialogDataList m_data;
public:
    ModalDialog m_modal;
    ui::Window m_parent;

    Dialog();

    virtual ~Dialog();

/*!
   start modal dialog box
   you need to use AddModalButton to select eIDOK eIDCANCEL buttons
 */
    EMessageBoxReturn DoModal();

    void EndModal(EMessageBoxReturn code);

    virtual ui::Window BuildDialog() = 0;

    virtual void exportData();

    virtual void importData();

    virtual void PreModal()
    {};

    virtual void PostModal(EMessageBoxReturn code)
    {};

    virtual void ShowDlg();

    virtual void HideDlg();

    void Create();

    void Destroy();

    ui::Window GetWidget()
    {
        return m_window;
    }

    const ui::Window GetWidget() const
    {
        return m_window;
    }

    ui::CheckButton addCheckBox(ui::VBox vbox, const char *name, const char *flag, Property<bool> const &cb);

    ui::CheckButton addCheckBox(ui::VBox vbox, const char *name, const char *flag, bool &data);

    void addCombo(ui::VBox vbox, const char *name, StringArrayRange values, Property<int> const &cb);

    void addCombo(ui::VBox vbox, const char *name, int &data, StringArrayRange values);

    void addSlider(ui::VBox vbox, const char *name, int &data, gboolean draw_value, const char *low, const char *high,
                   double value, double lower, double upper, double step_increment, double page_increment);

    void addRadio(ui::VBox vbox, const char *name, StringArrayRange names, Property<int> const &cb);

    void addRadio(ui::VBox vbox, const char *name, int &data, StringArrayRange names);

    void addRadioIcons(ui::VBox vbox, const char *name, StringArrayRange icons, Property<int> const &cb);

    void addRadioIcons(ui::VBox vbox, const char *name, int &data, StringArrayRange icons);

    ui::Widget addIntEntry(ui::VBox vbox, const char *name, Property<int> const &cb);

    ui::Widget addEntry(ui::VBox vbox, const char *name, int &data)
    {
        return addIntEntry(vbox, name, make_property(data));
    }

    ui::Widget addSizeEntry(ui::VBox vbox, const char *name, Property<std::size_t> const &cb);

    ui::Widget addEntry(ui::VBox vbox, const char *name, std::size_t &data)
    {
        return addSizeEntry(vbox, name, make_property(data));
    }

    ui::Widget addFloatEntry(ui::VBox vbox, const char *name, Property<float> const &cb);

    ui::Widget addEntry(ui::VBox vbox, const char *name, float &data)
    {
        return addFloatEntry(vbox, name, make_property(data));
    }

    ui::Widget addPathEntry(ui::VBox vbox, const char *name, bool browse_directory, Property<const char *> const &cb);

    ui::Widget addPathEntry(ui::VBox vbox, const char *name, CopiedString &data, bool directory);

    ui::SpinButton addSpinner(ui::VBox vbox, const char *name, int &data, double value, double lower, double upper);

    ui::SpinButton
    addSpinner(ui::VBox vbox, const char *name, double value, double lower, double upper, Property<int> const &cb);

    ui::SpinButton
    addSpinner(ui::VBox vbox, const char *name, double value, double lower, double upper, Property<float> const &cb);

protected:

    void AddBoolToggleData(ui::ToggleButton object, Property<bool> const &cb);

    void AddIntRadioData(ui::RadioButton object, Property<int> const &cb);

    void AddTextEntryData(ui::Entry object, Property<const char *> const &cb);

    void AddIntEntryData(ui::Entry object, Property<int> const &cb);

    void AddSizeEntryData(ui::Entry object, Property<std::size_t> const &cb);

    void AddFloatEntryData(ui::Entry object, Property<float> const &cb);

    void AddFloatSpinnerData(ui::SpinButton object, Property<float> const &cb);

    void AddIntSpinnerData(ui::SpinButton object, Property<int> const &cb);

    void AddIntAdjustmentData(ui::Adjustment object, Property<int> const &cb);

    void AddIntComboData(ui::ComboBox object, Property<int> const &cb);

    void AddDialogData(ui::ToggleButton object, bool &data);

    void AddDialogData(ui::RadioButton object, int &data);

    void AddDialogData(ui::Entry object, CopiedString &data);

    void AddDialogData(ui::Entry object, int &data);

    void AddDialogData(ui::Entry object, std::size_t &data);

    void AddDialogData(ui::Entry object, float &data);

    void AddDialogData(ui::SpinButton object, float &data);

    void AddDialogData(ui::SpinButton object, int &data);

    void AddDialogData(ui::Adjustment object, int &data);

    void AddDialogData(ui::ComboBox object, int &data);
};

#endif
