#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QLabel>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI Button Actions
    void onAddSlotClicked();
    void onCheckConflictsClicked();
    void onAddJobClicked();
    void onRunFCFSClicked();
    void onRunPriorityClicked();
    void onConvertPostfixClicked();
    void onConvertPrefixClicked(); // Added Prefix Slot

private:
    // UI Elements
    QTabWidget *tabWidget;

    // Timetable Tab
    QTableWidget *tableSlots;
    QComboBox *comboSlotType;
    QLineEdit *inputSlotName;
    QComboBox *comboSlotDay;
    QComboBox *comboSlotStart;
    QLineEdit *inputSlotRoom;

    // Jobs Tab
    QTableWidget *tableJobs;
    QComboBox *comboJobType;
    QLineEdit *inputJobName;
    QSpinBox *spinJobBurst;
    QSpinBox *spinJobArrival;

    // Utilities Tab
    QLineEdit *inputInfix;
    QLabel *lblPostfixResult;

    // Setup Functions
    void setupUI();
    void setupTimetableTab(QWidget *tab);
    void setupJobsTab(QWidget *tab);
    void setupUtilitiesTab(QWidget *tab);
    void setupHelpTab(QWidget *tab);
    void applyTheme();

    // Data Refreshers
    void refreshSlotTable();
    void refreshJobTable();
};

#endif // MAINWINDOW_H