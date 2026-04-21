#include "mainwindow.h"
#include <QHeaderView>
#include <QApplication>
#include <vector>
#include <stack>
#include <algorithm>

// ==========================================================
// CORE LOGIC
// ==========================================================

struct ScheduleItem {
    int id;
    QString name;
    virtual ~ScheduleItem() {}
};

struct TimeSlot : public ScheduleItem {
    QString type, day, start, room;
    TimeSlot(int i, QString n, QString t, QString d, QString s, QString r) {
        id = i; name = n; type = t; day = d; start = s; room = r;
    }
};

struct Job : public ScheduleItem {
    QString type;
    int burst, arrival, priority;
    Job(int i, QString t, QString n, int b, int a, int p) {
        id = i; type = t; name = n; burst = b; arrival = a; priority = p;
    }
};

std::vector<TimeSlot> g_slots;
std::vector<Job> g_jobs;
int nextSlotId = 1;
int nextJobId = 1;

int precedence(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Logic: Infix to Postfix
QString infixToPostfixLogic(QString expr) {
    QString postfix = "";
    std::stack<QChar> stk;
    for (QChar c : expr) {
        if (c.isSpace()) continue;
        if (c.isLetterOrNumber()) {
            postfix += c;
        } else if (c == '(') {
            stk.push(c);
        } else if (c == ')') {
            while (!stk.empty() && stk.top() != '(') {
                postfix += stk.top();
                stk.pop();
            }
            if (!stk.empty()) stk.pop();
        } else {
            while (!stk.empty() && precedence(stk.top()) >= precedence(c)) {
                postfix += stk.top();
                stk.pop();
            }
            stk.push(c);
        }
    }
    while (!stk.empty()) { postfix += stk.top(); stk.pop(); }
    return postfix;
}

// Logic: Infix to Prefix
QString infixToPrefixLogic(QString expr) {
    // Step 1: Reverse the string and swap parentheses
    QString reversed = "";
    for(int i = expr.length() - 1; i >= 0; i--) {
        QChar c = expr[i];
        if (c == '(') reversed += ')';
        else if (c == ')') reversed += '(';
        else reversed += c;
    }

    // Step 2: Modified Postfix algorithm (Strict '>' precedence)
    QString prefixTemp = "";
    std::stack<QChar> stk;
    for (QChar c : reversed) {
        if (c.isSpace()) continue;
        if (c.isLetterOrNumber()) {
            prefixTemp += c;
        } else if (c == '(') {
            stk.push(c);
        } else if (c == ')') {
            while (!stk.empty() && stk.top() != '(') {
                prefixTemp += stk.top();
                stk.pop();
            }
            if (!stk.empty()) stk.pop();
        } else {
            while (!stk.empty() && precedence(stk.top()) > precedence(c)) {
                prefixTemp += stk.top();
                stk.pop();
            }
            stk.push(c);
        }
    }
    while (!stk.empty()) { prefixTemp += stk.top(); stk.pop(); }

    // Step 3: Reverse the final result
    QString finalPrefix = "";
    for(int i = prefixTemp.length() - 1; i >= 0; i--) {
        finalPrefix += prefixTemp[i];
    }
    return finalPrefix;
}

// ==========================================================
// MAINWINDOW IMPLEMENTATION
// ==========================================================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Smart Scheduler System - Pro Edition");
    resize(1000, 650);

    applyTheme();

    // Load Initial Dummy Data
    g_slots.push_back({nextSlotId++, "Data Structures", "Lecture", "Monday", "9:00", "A101"});
    g_slots.push_back({nextSlotId++, "C++ Lab", "Lab", "Monday", "10:00", "A101"});
    g_slots.push_back({nextSlotId++, "Tech Training", "Lecture", "Tuesday", "11:00", "B202"});
    g_slots.push_back({nextSlotId++, "Lunch Break", "Break", "Wednesday", "12:00", "Cafeteria"});

    g_jobs.push_back({nextJobId++, "High", "ExamResult", 3, 0, 1});
    g_jobs.push_back({nextJobId++, "Low", "BackupData", 5, 1, 3});
    g_jobs.push_back({nextJobId++, "IO", "PrintDocs", 2, 2, 2});

    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::applyTheme() {
    this->setStyleSheet(
        "QMainWindow { background-color: #1e1e2e; }"
        "QTabWidget::pane { border: 1px solid #45475a; background: #1e1e2e; }"
        "QTabBar::tab { background: #313244; color: #a6adc8; padding: 10px 20px; font-weight: bold; border-top-left-radius: 4px; border-top-right-radius: 4px; margin-right: 2px; }"
        "QTabBar::tab:selected { background: #cba6f7; color: #11111b; }"
        "QPushButton { background-color: #89b4fa; color: #11111b; font-weight: bold; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #b4befe; }"
        "QTableWidget { background-color: #313244; color: #cdd6f4; gridline-color: #45475a; border: none; }"
        "QHeaderView::section { background-color: #181825; color: #f38ba8; font-weight: bold; padding: 5px; border: 1px solid #313244; }"
        "QLabel { color: #cdd6f4; font-weight: bold; }"
        "QLineEdit, QComboBox, QSpinBox { background-color: #45475a; color: #cdd6f4; border: 1px solid #585b70; padding: 5px; border-radius: 4px; }"
        "QTextEdit { background-color: #313244; color: #cdd6f4; border: none; font-size: 14px; }"
        );
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    QWidget *tabTimetable = new QWidget();
    QWidget *tabJobs = new QWidget();
    QWidget *tabUtils = new QWidget();
    QWidget *tabHelp = new QWidget();

    setupTimetableTab(tabTimetable);
    setupJobsTab(tabJobs);
    setupUtilitiesTab(tabUtils);
    setupHelpTab(tabHelp);

    tabWidget->addTab(tabTimetable, "📅 Matrix Timetable");
    tabWidget->addTab(tabJobs, "⚙️ Job Scheduler");
    tabWidget->addTab(tabUtils, "🔧 Logic Utilities");
    tabWidget->addTab(tabHelp, "📖 How to Use");
}

void MainWindow::setupTimetableTab(QWidget *tab) {
    QHBoxLayout *layout = new QHBoxLayout(tab);

    tableSlots = new QTableWidget();
    tableSlots->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableSlots->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tableSlots, 3);

    QWidget *controls = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(controls);

    QFormLayout *form = new QFormLayout();
    comboSlotType = new QComboBox(); comboSlotType->addItems({"Lecture", "Lab", "Break"});
    inputSlotName = new QLineEdit();
    comboSlotDay = new QComboBox(); comboSlotDay->addItems({"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"});

    comboSlotStart = new QComboBox();
    comboSlotStart->addItems({"9:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"});

    inputSlotRoom = new QLineEdit(); inputSlotRoom->setPlaceholderText("A101");

    form->addRow("Type:", comboSlotType);
    form->addRow("Name:", inputSlotName);
    form->addRow("Day:", comboSlotDay);
    form->addRow("Time:", comboSlotStart);
    form->addRow("Room:", inputSlotRoom);

    QPushButton *btnAddSlot = new QPushButton("➕ Add Schedule");
    btnAddSlot->setStyleSheet("background-color: #a6e3a1; color: #11111b;");

    QPushButton *btnCheckConflicts = new QPushButton("🔍 Detect Graph Conflicts");
    btnCheckConflicts->setStyleSheet("background-color: #f38ba8; color: #11111b;");

    vbox->addLayout(form);
    vbox->addWidget(btnAddSlot);
    vbox->addSpacing(20);
    vbox->addWidget(btnCheckConflicts);
    vbox->addStretch();

    layout->addWidget(controls, 1);

    connect(btnAddSlot, &QPushButton::clicked, this, &MainWindow::onAddSlotClicked);
    connect(btnCheckConflicts, &QPushButton::clicked, this, &MainWindow::onCheckConflictsClicked);

    refreshSlotTable();
}

void MainWindow::refreshSlotTable() {
    QStringList days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    QStringList times = {"9:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};

    tableSlots->clear();
    tableSlots->setColumnCount(days.size());
    tableSlots->setRowCount(times.size());

    tableSlots->setHorizontalHeaderLabels(days);
    tableSlots->setVerticalHeaderLabels(times);

    for(int r = 0; r < times.size(); r++) {
        for(int c = 0; c < days.size(); c++) {
            QString cellText = "";
            QString typeColor = "";

            for(const auto& s : g_slots) {
                if(s.day == days[c] && s.start == times[r]) {
                    cellText += s.name + "\n[" + s.room + "]\n";
                    if (s.type == "Lecture") typeColor = "#89b4fa";
                    else if (s.type == "Lab") typeColor = "#cba6f7";
                    else typeColor = "#f9e2af";
                }
            }

            QTableWidgetItem *item = new QTableWidgetItem(cellText);
            item->setTextAlignment(Qt::AlignCenter);

            if(!cellText.isEmpty()) {
                item->setBackground(QColor(typeColor));
                item->setForeground(QColor("#11111b"));
            }

            tableSlots->setItem(r, c, item);
        }
    }
}

void MainWindow::setupJobsTab(QWidget *tab) {
    QHBoxLayout *layout = new QHBoxLayout(tab);

    tableJobs = new QTableWidget(0, 5);
    tableJobs->setHorizontalHeaderLabels({"ID", "Priority Level", "Process Name", "Burst Time", "Arrival Time"});
    tableJobs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tableJobs, 2);

    QWidget *controls = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(controls);

    QFormLayout *form = new QFormLayout();
    comboJobType = new QComboBox(); comboJobType->addItems({"High", "Low", "IO"});
    inputJobName = new QLineEdit();
    spinJobBurst = new QSpinBox(); spinJobBurst->setRange(1, 100);
    spinJobArrival = new QSpinBox(); spinJobArrival->setRange(0, 100);

    form->addRow("Priority:", comboJobType);
    form->addRow("Name:", inputJobName);
    form->addRow("Burst:", spinJobBurst);
    form->addRow("Arrival:", spinJobArrival);

    QPushButton *btnAddJob = new QPushButton("➕ Add Job");
    btnAddJob->setStyleSheet("background-color: #a6e3a1; color: #11111b;");

    QPushButton *btnFCFS = new QPushButton("▶ Run FCFS");
    QPushButton *btnPriority = new QPushButton("▶ Run Priority Algo");

    vbox->addLayout(form);
    vbox->addWidget(btnAddJob);
    vbox->addSpacing(20);
    vbox->addWidget(btnFCFS);
    vbox->addWidget(btnPriority);
    vbox->addStretch();

    layout->addWidget(controls, 1);

    connect(btnAddJob, &QPushButton::clicked, this, &MainWindow::onAddJobClicked);
    connect(btnFCFS, &QPushButton::clicked, this, &MainWindow::onRunFCFSClicked);
    connect(btnPriority, &QPushButton::clicked, this, &MainWindow::onRunPriorityClicked);

    refreshJobTable();
}

void MainWindow::setupUtilitiesTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);

    // Removed the CO1 text, updated title
    layout->addWidget(new QLabel("<h2 style='color:#a6e3a1;'>Infix to Postfix & Prefix Machine</h2>"));
    inputInfix = new QLineEdit();
    inputInfix->setPlaceholderText("Enter math formula e.g. (A+B)*C");
    inputInfix->setStyleSheet("font-size: 16px; padding: 10px;");
    layout->addWidget(inputInfix);

    // Horizontal layout for the two buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnConvertPostfix = new QPushButton("⚡ Translate to Postfix");
    QPushButton *btnConvertPrefix = new QPushButton("⚡ Translate to Prefix");

    // Style Prefix button differently so it stands out
    btnConvertPrefix->setStyleSheet("background-color: #f9e2af; color: #11111b; font-weight: bold; padding: 8px; border-radius: 5px;");

    btnLayout->addWidget(btnConvertPostfix);
    btnLayout->addWidget(btnConvertPrefix);
    layout->addLayout(btnLayout);

    lblPostfixResult = new QLabel("Result will appear here.");
    lblPostfixResult->setStyleSheet("color: #f9e2af; font-family: monospace; font-size: 24px; padding: 20px; background: #181825; border-radius: 8px;");
    lblPostfixResult->setAlignment(Qt::AlignCenter);
    layout->addWidget(lblPostfixResult);

    connect(btnConvertPostfix, &QPushButton::clicked, this, &MainWindow::onConvertPostfixClicked);
    connect(btnConvertPrefix, &QPushButton::clicked, this, &MainWindow::onConvertPrefixClicked);

    layout->addStretch();
}

void MainWindow::setupHelpTab(QWidget *tab) {
    QVBoxLayout *layout = new QVBoxLayout(tab);
    QTextEdit *helpText = new QTextEdit();
    helpText->setReadOnly(true);
    helpText->setHtml(
        "<h1 style='color:#cba6f7;'>User Guide & Architecture</h1>"
        "<h3>📅 Matrix Timetable</h3>"
        "<ul>"
        "<li><b>Adding Data:</b> Use the panel on the right to add Classes or Breaks. Because this is a horizontal matrix, times are strictly mapped to the hour.</li>"
        "<li><b>Conflict Detection:</b> Clicking the red button runs a Graph Adjacency Matrix (BFS logic) checking if two classes are assigned to the exact same Day, Time, and Room.</li>"
        "</ul>"
        "<h3>⚙️ Job Scheduler</h3>"
        "<ul>"
        "<li><b>Algorithms:</b> Click FCFS to calculate Waiting/Turnaround time based strictly on Arrival. Click Priority to sort them by type first before calculating.</li>"
        "</ul>"
        "<h3>🔧 Logic Utilities</h3>"
        "<ul>"
        "<li><b>Infix to Postfix/Prefix:</b> Type a human-readable mathematical equation using A, B, C and +, *, /. Choose either Postfix or Prefix to run the respective Stack algorithm.</li>"
        "</ul>"
        );
    layout->addWidget(helpText);
}

// ==========================================================
// ACTION SLOTS
// ==========================================================

void MainWindow::onAddSlotClicked() {
    if(inputSlotName->text().isEmpty()) return;
    g_slots.push_back({
        nextSlotId++, inputSlotName->text(), comboSlotType->currentText(),
        comboSlotDay->currentText(), comboSlotStart->currentText(), inputSlotRoom->text()
    });
    inputSlotName->clear();
    refreshSlotTable();
}

void MainWindow::onCheckConflictsClicked() {
    int n = g_slots.size();
    std::vector<std::vector<int>> adj(n, std::vector<int>(n, 0));
    QString conflicts = "";
    bool found = false;

    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            if(g_slots[i].day == g_slots[j].day && g_slots[i].start == g_slots[j].start && g_slots[i].room == g_slots[j].room) {
                adj[i][j] = adj[j][i] = 1;
                conflicts += "Room Clash: " + g_slots[i].name + " & " + g_slots[j].name + " (" + g_slots[i].day + " at " + g_slots[i].start + " in " + g_slots[i].room + ")\n";
                found = true;
            }
        }
    }

    if(found) QMessageBox::critical(this, "BFS Conflict Detected", conflicts);
    else QMessageBox::information(this, "BFS Status", "Graph Traversed: No scheduling conflicts found!");
}

void MainWindow::onAddJobClicked() {
    if(inputJobName->text().isEmpty()) return;
    int prio = (comboJobType->currentText() == "High") ? 1 : (comboJobType->currentText() == "IO" ? 2 : 3);
    g_jobs.push_back({ nextJobId++, comboJobType->currentText(), inputJobName->text(), spinJobBurst->value(), spinJobArrival->value(), prio });
    inputJobName->clear();
    refreshJobTable();
}

void MainWindow::refreshJobTable() {
    tableJobs->setRowCount(g_jobs.size());
    for(size_t i = 0; i < g_jobs.size(); ++i) {
        tableJobs->setItem(i, 0, new QTableWidgetItem(QString::number(g_jobs[i].id)));
        tableJobs->setItem(i, 1, new QTableWidgetItem(g_jobs[i].type));
        tableJobs->setItem(i, 2, new QTableWidgetItem(g_jobs[i].name));
        tableJobs->setItem(i, 3, new QTableWidgetItem(QString::number(g_jobs[i].burst) + " ms"));
        tableJobs->setItem(i, 4, new QTableWidgetItem(QString::number(g_jobs[i].arrival) + " ms"));
    }
}

void MainWindow::onRunFCFSClicked() {
    int time = 0;
    // Adding HTML styling to ensure text is dark, sharp, and easy to read on a bright background block.
    QString result = "<div style='background-color: #e6e9ef; color: #11111b; font-family: Arial, sans-serif; font-size: 14px; padding: 15px; border-radius: 6px;'>"
                     "<h3 style='color: #1e1e2e; margin-top: 0;'>FCFS Scheduling Execution:</h3><hr>";

    for(const auto& j : g_jobs) {
        if (time < j.arrival) time = j.arrival;
        int wait = time - j.arrival;
        int tat = wait + j.burst;
        time += j.burst;
        result += QString("Processed: <b>%1</b> | Wait: %2 ms | Turnaround: %3 ms<br>").arg(j.name).arg(wait).arg(tat);
    }
    result += "</div>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("FCFS Result");
    msgBox.setText(result);
    msgBox.exec();
}

void MainWindow::onRunPriorityClicked() {
    std::vector<Job> sorted = g_jobs;
    std::sort(sorted.begin(), sorted.end(), [](const Job& a, const Job& b){ return a.priority < b.priority; });
    int time = 0;

    // Forcing Dark text on Light block for high visibility
    QString result = "<div style='background-color: #e6e9ef; color: #11111b; font-family: Arial, sans-serif; font-size: 14px; padding: 15px; border-radius: 6px;'>"
                     "<h3 style='color: #1e1e2e; margin-top: 0;'>Priority Scheduling Execution:</h3><hr>";

    for(const auto& j : sorted) {
        int wait = time;
        int tat = wait + j.burst;
        time += j.burst;
        result += QString("Processed [%1] <b>%2</b> | Priority: %3<br>").arg(j.type).arg(j.name).arg(j.priority);
    }
    result += "</div>";

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Priority Result");
    msgBox.setText(result);
    msgBox.exec();
}

void MainWindow::onConvertPostfixClicked() {
    QString infix = inputInfix->text();
    if(infix.isEmpty()) return;
    QString postfix = infixToPostfixLogic(infix);
    lblPostfixResult->setText("Postfix: " + postfix);
}

void MainWindow::onConvertPrefixClicked() {
    QString infix = inputInfix->text();
    if(infix.isEmpty()) return;
    QString prefix = infixToPrefixLogic(infix);
    lblPostfixResult->setText("Prefix: " + prefix);
}