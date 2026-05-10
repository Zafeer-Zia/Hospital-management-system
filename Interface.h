#pragma once
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cstring>
#include <cstdio>
#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"
#include "Validators.h"
#include "Storage.h"
#include "Exceptions.h"
//new changes by Ta(must implement)
//new changes to implement 
//func to remove and add patients
void get_current_date(char* buf);

namespace MC
{
    const sf::Color BG{ 15, 40, 80 };
    const sf::Color PANEL{ 20, 55, 105 };
    const sf::Color PANEL2{ 25, 65, 120 };
    const sf::Color CARD{ 15, 40, 80 };
    const sf::Color CARD_HOV{ 20, 55, 105 };
    const sf::Color ACCENT{ 255, 255, 255 };
    const sf::Color ACCENT2{ 200, 220, 255 };
    const sf::Color CIRCLE{ 25, 65, 130 };
    const sf::Color TITLE{ 255, 255, 255 };
    const sf::Color TEXT{ 220, 235, 255 };
    const sf::Color SUBTEXT{ 120, 155, 210 };
    const sf::Color BORDER{ 40, 90, 170 };
    const sf::Color GREEN{ 255, 255, 255 };
    const sf::Color RED{ 180, 210, 255 };
    const sf::Color WHITE{ 255, 255, 255 };
    const sf::Color INPUT_BG{ 12, 35, 70 };
    const sf::Color INPUT_BOR{ 60, 110, 190 };
}

template<typename T> class Storage;
class Patient;
class Doctor;
class Admin;
class Appointment;
class Bill;
class Prescription;
class FileHandler;
class Validators;

extern Storage<Patient>      patients;
extern Storage<Doctor>       doctors;
extern Storage<Appointment>  appointments;
extern Storage<Bill>         bills;
extern Storage<Prescription> prescriptions;
extern FileHandler           fh;
extern Validators            validator;





static void scopy(char* dst, const char* src, int maxlen)
{
    int i = 0;
    while (src[i] && i < maxlen - 1)
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}



static bool smatch(const char* a, const char* b)
{
    int i = 0;
    while (a[i] && b[i])
    {
        if (a[i] != b[i])
        {
            return false;
        }
        i++;
    }
    return a[i] == '\0' && b[i] == '\0';
}




static int slen(const char* s)
{
    int i = 0;
    while (s[i])
    {
        i++;
    }
    return i;
}





struct Rect
{
    float x, y, w, h;
    bool contains(float px, float py) const
    {
        return px >= x && px <= x + w && py >= y && py <= y + h;
    }
};




static void drawRect(sf::RenderWindow& win, Rect r, sf::Color fill, sf::Color outline = { 0,0,0,0 }, float thickness = 0)
{
    sf::RectangleShape s({ r.w, r.h });
    s.setPosition({ r.x, r.y });
    s.setFillColor(fill);
    if (thickness > 0)
    {
        s.setOutlineThickness(thickness);
        s.setOutlineColor(outline);
    }
    win.draw(s);
}


//curved edges boxes ki
static void drawRounded(sf::RenderWindow& win, Rect r, sf::Color fill, float radius = 10.f, sf::Color outline = { 0,0,0,0 }, float outThick = 0)
{
    radius = std::min(radius, std::min(r.w, r.h) * 0.5f);

    sf::RectangleShape center({ r.w - 2 * radius, r.h });
    center.setPosition({ r.x + radius, r.y });
    center.setFillColor(fill);
    win.draw(center);

    sf::RectangleShape left({ radius, r.h - 2 * radius });
    left.setPosition({ r.x, r.y + radius });
    left.setFillColor(fill);
    win.draw(left);

    sf::RectangleShape right({ radius, r.h - 2 * radius });
    right.setPosition({ r.x + r.w - radius, r.y + radius });
    right.setFillColor(fill);
    win.draw(right);

    const int segs = 16;
    const float PI = 3.14159265f;
    struct Corner { float cx, cy, startAngle; };
    Corner corners[4] = {
        {r.x + radius,       r.y + radius,       PI},
        {r.x + r.w - radius, r.y + radius,       -PI / 2.f},
        {r.x + r.w - radius, r.y + r.h - radius, 0.f},
        {r.x + radius,       r.y + r.h - radius, PI / 2.f}
    };
    for (int c = 0; c < 4; c++)
    {
        sf::VertexArray fan(sf::PrimitiveType::TriangleFan, segs + 2);
        fan[0].position = { corners[c].cx, corners[c].cy };
        fan[0].color = fill;
        for (int s = 0; s <= segs; s++)
        {
            float angle = corners[c].startAngle + (PI / 2.f) * s / segs;
            fan[s + 1].position = {
                corners[c].cx + std::cos(angle) * radius,
                corners[c].cy + std::sin(angle) * radius
            };
            fan[s + 1].color = fill;
        }
        win.draw(fan);
    }

    if (outThick > 0)
    {
        drawRounded(win, { r.x - outThick, r.y - outThick, r.w + outThick * 2, r.h + outThick * 2 }, outline, radius);
        drawRounded(win, r, fill, radius);
    }
}






static void drawCircle(sf::RenderWindow& win, float cx, float cy, float radius, sf::Color fill)
{
    sf::CircleShape c(radius);
    c.setPosition({ cx - radius, cy - radius });
    c.setFillColor(fill);
    win.draw(c);
}

static void drawText(sf::RenderWindow& win, sf::Font& font, const char* str, float x, float y, unsigned int size, sf::Color col, bool centerX = false, float centerW = 0)
{
    sf::Text t(font, str, size);
    t.setFillColor(col);
    if (centerX)
    {
        float tw = t.getLocalBounds().size.x;
        x = x + (centerW - tw) / 2.f;
    }
    t.setPosition({ x, y });
    win.draw(t);
}

static void drawDivider(sf::RenderWindow& win, float x, float y, float w, sf::Color col)
{
    sf::RectangleShape d({ w, 1.f });
    d.setPosition({ x, y });
    d.setFillColor(col);
    win.draw(d);
}

struct InputBox
{
    Rect  rect;
    char  label[64];
    char  buf[256];
    bool  focused;
    bool  password;
    int   maxLen;

    InputBox() : focused(false), password(false), maxLen(50)
    {
        label[0] = '\0';
        buf[0] = '\0';
    }

    void draw(sf::RenderWindow& win, sf::Font& font) const
    {
        drawText(win, font, label, rect.x, rect.y - 20, 14, MC::SUBTEXT);
        drawRounded(win, rect, MC::INPUT_BG, 22.f, MC::INPUT_BOR, 1.f);

        char display[256];
        if (password)
        {
            int n = slen(buf);
            for (int i = 0; i < n; i++)
            {
                display[i] = '*';
            }
            display[n] = '\0';
        }
        else
        {
            scopy(display, buf, 256);
        }
        char withCursor[258];
        scopy(withCursor, display, 258);
        if (focused)
        {
            int n = slen(withCursor);
            withCursor[n] = '|';
            withCursor[n + 1] = '\0';
        }
        drawText(win, font, withCursor, rect.x + 12, rect.y + rect.h / 2.f - 10, 16, MC::TEXT);
    }

    bool handleEvent(const sf::Event& event)
    {
        if (!focused)
        {
            return false;
        }
        if (auto* kp = event.getIf<sf::Event::KeyPressed>())
        {
            if (kp->code == sf::Keyboard::Key::Backspace)
            {
                int n = slen(buf);
                if (n > 0)
                {
                    buf[n - 1] = '\0';
                }
                return true;
            }
        }
        if (auto* te = event.getIf<sf::Event::TextEntered>())
        {
            uint32_t c = te->unicode;
            if (c >= 32 && c < 127)
            {
                int n = slen(buf);
                if (n < maxLen - 1)
                {
                    buf[n] = (char)c;
                    buf[n + 1] = '\0';
                }
                return true;
            }
        }
        return false;
    }

    void checkClick(float mx, float my)
    {
        focused = rect.contains(mx, my);
    }
};

struct Button
{
    Rect      rect;
    char      label[64];
    sf::Color col;
    int       id;

    Button() : col(MC::ACCENT), id(0)
    {
        label[0] = '\0';
    }

    bool isHovered(float mx, float my) const
    {
        return rect.contains(mx, my);
    }

    void draw(sf::RenderWindow& win, sf::Font& font, float mx, float my) const
    {
        bool hov = isHovered(mx, my);
        sf::Color fill = hov ? MC::CARD_HOV : MC::CARD;
        drawRounded(win, rect, fill, 22.f, MC::BORDER, 1.f);

        drawText(win, font, label, rect.x + 16.f, rect.y + rect.h / 2.f - 10, 16, hov ? MC::WHITE : MC::TEXT);
    }
};

struct ScrollList
{
    Rect  rect;
    char  lines[64][256];
    int   count;
    int   scroll;
    int   rowH;

    ScrollList() : count(0), scroll(0), rowH(30) {}

    void clear()
    {
        count = 0;
        scroll = 0;
    }

    void addLine(const char* s)
    {
        if (count < 64)
        {
            scopy(lines[count++], s, 256);
        }
    }

    void draw(sf::RenderWindow& win, sf::Font& font) const
    {
        drawRounded(win, rect, MC::BG, 10.f, MC::BG, 0.f);
        int visible = (int)(rect.h / rowH);
        for (int i = 0; i < visible && (scroll + i) < count; i++)
        {
            float ry = rect.y + i * rowH;
            float rh = std::min((float)rowH, rect.y + rect.h - ry - 1.f);
            if (rh <= 0)
            {
                break;
            }
            sf::Color rc = MC::BG;
            drawRect(win, { rect.x + 1.f, ry, rect.w - 2.f, rh }, rc);
            drawRect(win, { rect.x + 1.f, ry, 3.f, rh }, MC::BORDER);
            drawText(win, font, lines[scroll + i], rect.x + 14, ry + rh / 2.f - 9, 13, MC::TEXT);
        }
        if (count > visible && visible > 0)
        {
            float trackH = rect.h - 4.f;
            float thumbH = trackH * visible / count;
            float thumbY = rect.y + 2.f + trackH * scroll / count;
            drawRect(win, { rect.x + rect.w - 6.f, rect.y + 2.f, 4.f, trackH }, sf::Color(60, 40, 80));
            drawRect(win, { rect.x + rect.w - 6.f, thumbY, 4.f, thumbH }, MC::CIRCLE);
        }
        if (count == 0)
        {
            drawText(win, font, "No records found.", rect.x, rect.y + rect.h / 2.f - 10, 15, MC::SUBTEXT, true, rect.w);
        }
    }

    void scroll_up()
    {
        if (scroll > 0)
        {
            scroll--;
        }
    }

    void scroll_down()
    {
        int visible = (int)(rect.h / rowH);
        if (scroll + visible < count)
        {
            scroll++;
        }
    }

    void handleScroll(const sf::Event& ev)
    {
        if (auto* sw = ev.getIf<sf::Event::MouseWheelScrolled>())
        {
            if (sw->delta > 0)
            {
                scroll_up();
            }
            else
            {
                scroll_down();
            }
        }
    }
};

struct Notif
{
    char  msg[256];
    bool  visible;
    bool  isError;
    int   timer;

    Notif() : visible(false), isError(false), timer(0)
    {
        msg[0] = '\0';
    }

    void show(const char* m, bool err = false)
    {
        scopy(msg, m, 256);
        visible = true;
        isError = err;
        timer = 180;
    }

    void tick()
    {
        if (visible && --timer <= 0)
        {
            visible = false;
        }
    }

    void draw(sf::RenderWindow& win, sf::Font& font, float w) const
    {
        if (!visible)
        {
            return;
        }
        sf::Color c = isError ? MC::RED : MC::GREEN;
        drawRect(win, { 0, 0, w, 36.f }, sf::Color(c.r, c.g, c.b, 210));
        drawText(win, font, msg, 0, 8, 16, MC::WHITE, true, w);
    }
};

enum class Screen
{
    ROLE_SELECT,
    LOGIN,
    PATIENT_MENU,
    DOCTOR_MENU,
    ADMIN_MENU,
    BOOK_STEP1,
    BOOK_STEP2,
    CANCEL_APPT,
    VIEW_APPTS,
    VIEW_RECORDS,
    VIEW_BILLS,
    PAY_BILL,
    TOPUP,
    TODAY_APPTS,
    MARK_COMPLETE,
    MARK_NOSHOW,
    WRITE_PRESC,
    VIEW_PAT_HIST,
    ADD_DOCTOR,
    REMOVE_DOCTOR,
    VIEW_ALL_PATIENTS,
    VIEW_ALL_DOCTORS,
    VIEW_ALL_APPTS,
    VIEW_UNPAID,
    DISCHARGE,
    VIEW_SECLOG,
    DAILY_REPORT,
    ADD_PATIENT,
    REMOVE_PATIENT,
    EXIT
};

class MediCoreUI
{
public:
    sf::RenderWindow win;
    sf::Font         font;
    bool             fontOK;

    Screen  screen;
    int     loginRole;
    int     loginAttempts;

    Patient* curPatient;
    Doctor* curDoctor;

    Notif       notif;
    InputBox    inp[6];
    Button      btns[16];
    int         btnCount;
    ScrollList  list;

    char  tmpSpec[51];
    int   tmpDoctorId;
    char  tmpDate[11];
    char  tmpSlot[6];
    int   tmpAppointmentId;
    int   tmpBillId;

    char  availableSlots[8][6];
    int   slotCount;
    int   selectedSlot;

    int   doctorResults[64];
    int   doctorResultCount;
    int   selectedDoctorIdx;

    float W, H;

    sf::Texture             plusTex;
    std::optional<sf::Sprite> plusSprite;
    bool                    plusLoaded;

    MediCoreUI() :
        win(sf::VideoMode({ 900, 620 }), "MediCore "),
        fontOK(false),
        screen(Screen::ROLE_SELECT),
        loginRole(0), loginAttempts(0),
        curPatient(nullptr), curDoctor(nullptr),
        btnCount(0),
        tmpDoctorId(0), slotCount(0), selectedSlot(-1),
        doctorResultCount(0), selectedDoctorIdx(-1),
        plusLoaded(false)
    {
        win.setFramerateLimit(60);
        fontOK = font.openFromFile("C:/Windows/Fonts/arial.ttf");
        W = 900;
        H = 620;
        plusLoaded = plusTex.loadFromFile("plus.png");
        if (plusLoaded)
        {
            plusSprite.emplace(plusTex);
        }
        tmpSpec[0] = '\0';
        tmpDate[0] = '\0';
        tmpSlot[0] = '\0';
        goRoleSelect();
    }

    void run()
    {
        while (win.isOpen())
        {
            while (auto ev = win.pollEvent())
            {
                if (ev->is<sf::Event::Closed>())
                {
                    win.close();
                    return;
                }
                handleEvent(*ev);
            }
            notif.tick();
            win.clear(MC::BG);
            draw();
            win.display();
        }
    }

    void goRoleSelect()
    {
        screen = Screen::ROLE_SELECT;
        btnCount = 0;
        clearInputs();
    }

    void goLogin(int role)
    {
        loginRole = role;
        loginAttempts = 0;
        screen = Screen::LOGIN;
        btnCount = 0;
        clearInputs();
        setupInput(0, "User ID", false, false);
        setupInput(1, "Password", false, true);
        addBtn(0, "Login", { W * 0.30f, H * 0.62f, W * 0.40f, 44 }, MC::ACCENT);
        addBtn(1, "Back", { W * 0.30f, H * 0.74f, W * 0.40f, 36 }, MC::CIRCLE);
    }

    void goPatientMenu()
    {
        screen = Screen::PATIENT_MENU;
        btnCount = 0;
        clearInputs();
        const char* labels[] = {
            "Book Appointment", "Cancel Appointment",
            "View My Appointments", "View Medical Records",
            "View My Bills", "Pay Bill",
            "Top Up Balance", "Logout"
        };
        sf::Color cols[] = {
            MC::ACCENT, MC::RED, MC::GREEN, MC::ACCENT,
            MC::ACCENT, MC::GREEN, MC::ACCENT, MC::SUBTEXT
        };
        layoutMenuCards(labels, cols, 8);
    }

    void goDoctorMenu()
    {
        screen = Screen::DOCTOR_MENU;
        btnCount = 0;
        clearInputs();
        const char* labels[] = {
            "Today's Appointments", "Mark Appointment Complete",
            "Mark Appointment No-Show", "Write Prescription",
            "View Patient History", "Logout"
        };
        sf::Color cols[] = {
            MC::ACCENT, MC::GREEN, MC::ACCENT, MC::ACCENT, MC::GREEN, MC::SUBTEXT
        };
        layoutMenuCards(labels, cols, 6);
    }

    void goAdminMenu()
    {
        screen = Screen::ADMIN_MENU;
        btnCount = 0;
        clearInputs();
        const char* labels[] = {
            "Add Doctor", "Remove Doctor", "View All Patients",
            "View All Doctors", "View All Appointments",
            "View Unpaid Bills", "Discharge Patient",
            "View Security Log", "Daily Report",
            "Add Patient", "Remove Patient", "Logout"
        };
        sf::Color cols[] = {
            MC::GREEN, MC::RED, MC::ACCENT, MC::ACCENT, MC::ACCENT,
            MC::ACCENT, MC::RED, MC::SUBTEXT, MC::ACCENT,
            MC::GREEN, MC::RED, MC::SUBTEXT
        };
        layoutMenuCards(labels, cols, 12);
    }

    void draw()
    {
        switch (screen)
        {
        case Screen::ROLE_SELECT:
            drawRoleSelect();
            break;
        case Screen::LOGIN:
            drawLogin();
            break;
        case Screen::PATIENT_MENU:
            drawMenuScreen("Patient Portal", MC::ACCENT);
            break;
        case Screen::DOCTOR_MENU:
            drawMenuScreen("Doctor Portal", MC::GREEN);
            break;
        case Screen::ADMIN_MENU:
            drawMenuScreen("Admin Panel", MC::ACCENT);
            break;
        case Screen::BOOK_STEP1:
            drawBookStep1();
            break;
        case Screen::BOOK_STEP2:
            drawBookStep2();
            break;
        case Screen::CANCEL_APPT:
            drawWithListAndInput("Cancel Appointment");
            break;
        case Screen::VIEW_APPTS:
            drawListOnly("My Appointments");
            break;
        case Screen::VIEW_RECORDS:
            drawListOnly("My Medical Records");
            break;
        case Screen::VIEW_BILLS:
            drawListOnly("My Bills");
            break;
        case Screen::PAY_BILL:
            drawWithListAndInput("Pay Bill");
            break;
        case Screen::TOPUP:
            drawFormScreen("Top Up Balance");
            break;
        case Screen::TODAY_APPTS:
            drawListOnly("Today's Appointments");
            break;
        case Screen::MARK_COMPLETE:
            drawWithListAndInput("Mark Complete");
            break;
        case Screen::MARK_NOSHOW:
            drawWithListAndInput("Mark No-Show");
            break;
        case Screen::WRITE_PRESC:
            drawFormScreen("Write Prescription");
            break;
        case Screen::VIEW_PAT_HIST:
            drawWithListAndInput("Patient History");
            break;
        case Screen::ADD_DOCTOR:
            drawFormScreen("Add Doctor");
            break;
        case Screen::REMOVE_DOCTOR:
            drawWithListAndInput("Remove Doctor");
            break;
        case Screen::VIEW_ALL_PATIENTS:
            drawListOnly("All Patients");
            break;
        case Screen::VIEW_ALL_DOCTORS:
            drawListOnly("All Doctors");
            break;
        case Screen::VIEW_ALL_APPTS:
            drawListOnly("All Appointments");
            break;
        case Screen::VIEW_UNPAID:
            drawListOnly("Unpaid Bills");
            break;
        case Screen::DISCHARGE:
            drawWithListAndInput("Discharge Patient");
            break;
        case Screen::VIEW_SECLOG:
            drawListOnly("Security Log");
            break;
        case Screen::DAILY_REPORT:
            drawListOnly("Daily Report");
            break;
        case Screen::ADD_PATIENT:
            drawFormScreen("Add Patient");
            break;
        case Screen::REMOVE_PATIENT:
            drawWithListAndInput("Remove Patient");
            break;
        default:
            break;
        }
        notif.draw(win, font, W);
    }

    void drawRoleSelect()
    {
        drawRect(win, { 0, 0, W, H }, MC::BG);

        if (fontOK)
        {
            sf::Text titleText(font, "MEDICORE", 52);
            titleText.setFillColor(MC::TITLE);
            titleText.setStyle(sf::Text::Italic);
            float tw = titleText.getLocalBounds().size.x;
            titleText.setPosition({ (W - tw) / 2.f, 28.f });
            win.draw(titleText);
        }

        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;

        const char* labels[] = { "Patient Login", "Doctor Login", "Admin Login", "Exit" };
        const char* descs[] = { "Appointments & records", "Schedule & prescriptions", "System administration", "Close application" };
        sf::Color   accents[] = { MC::ACCENT, MC::ACCENT, MC::ACCENT, MC::SUBTEXT };

        float cardX = 18.f, cardW = W - 36.f;
        float cardH = 60.f, cardGap = 10.f;
        float startY = 160.f;
        for (int i = 0; i < 4; i++)
        {
            float by = startY + i * (cardH + cardGap);
            bool  hov = (mx >= cardX && mx <= cardX + cardW && my >= by && my <= by + cardH);

            sf::Color fill = hov ? MC::CARD_HOV : MC::CARD;
            drawRounded(win, { cardX, by, cardW, cardH }, fill, 42.f, MC::BORDER, 1.f);

            if (fontOK)
            {
                drawText(win, font, labels[i], cardX + 72.f, by + cardH / 2.f - 12.f, 20, hov ? MC::WHITE : MC::TEXT);
            }
        }
    }

    void drawLogin()
    {
        drawRect(win, { 0, 0, W, H }, MC::BG);

        if (fontOK)
        {
            sf::Text titleText(font, "MEDICORE", 46);
            titleText.setFillColor(MC::TITLE);
            titleText.setStyle(sf::Text::Italic);
            float tw = titleText.getLocalBounds().size.x;
            titleText.setPosition({ (W - tw) / 2.f, 22.f });
            win.draw(titleText);
        }

        const char* roleNames[] = { "", "Patient Login", "Doctor Login", "Admin Login" };
        if (fontOK)
        {
            drawText(win, font, roleNames[loginRole], 28.f, 122.f, 20, MC::TEXT);
            drawText(win, font, "Enter your credentials below", 28.f, 148.f, 13, MC::SUBTEXT);
        }

        float px = W * 0.20f, py = 170.f, pw = W * 0.60f, ph = 310.f;
        drawRect(win, { px, py, pw, ph }, MC::PANEL, MC::BORDER, 1.f);

        inp[0].rect = { px + 30.f, py + 60.f,  pw - 60.f, 44.f };
        inp[1].rect = { px + 30.f, py + 140.f, pw - 60.f, 44.f };
        inp[0].draw(win, font);
        inp[1].draw(win, font);

        btns[0].rect = { px + 30.f, py + 210.f, pw - 60.f, 46.f };
        btns[1].rect = { px + 30.f, py + 264.f, pw - 60.f, 36.f };

        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx2 = (float)mp.x, my2 = (float)mp.y;
        for (int i = 0; i < 2; i++)
        {
            btns[i].draw(win, font, mx2, my2);
        }
    }

    void drawMenuScreen(const char* title, sf::Color accent)
    {
        drawRect(win, { 0, 0, W, H }, MC::BG);

        if (fontOK)
        {
            sf::Text titleText(font, "MEDICORE", 38);
            titleText.setFillColor(MC::TITLE);
            titleText.setStyle(sf::Text::Italic);
            titleText.setPosition({ 24.f, 18.f });
            win.draw(titleText);
        }

        if (fontOK)
        {
            drawText(win, font, title, 24.f, 78.f, 20, MC::TEXT);

            char info[128] = "";
            if (curPatient)
            {
                snprintf(info, 128, "Welcome, %s  |  Balance: PKR %.0f", curPatient->getName(), curPatient->getBalance());
            }
            else if (curDoctor)
            {
                snprintf(info, 128, "Welcome, Dr. %s  |  %s", curDoctor->getName(), curDoctor->getSpecialization());
            }
            else
            {
                snprintf(info, 128, "Welcome, Administrator");
            }
            drawText(win, font, info, 24.f, 104.f, 13, MC::SUBTEXT);
        }

        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;
        for (int i = 0; i < btnCount; i++)
        {
            btns[i].draw(win, font, mx, my);
        }
    }

    void drawBookStep1()
    {
        drawPageHeader("Book Appointment", "Step 1: Search by specialization");
        inp[0].rect = { W * 0.05f, H * 0.32f, W * 0.55f, 44.f };
        inp[0].draw(win, font);
        list.rect = { W * 0.05f, H * 0.50f, W * 0.90f, H * 0.38f };
        list.draw(win, font);
        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;
        for (int i = 0; i < btnCount; i++)
        {
            btns[i].draw(win, font, mx, my);
        }
    }

    void drawBookStep2()
    {
        drawPageHeader("Book Appointment", "Step 2: Pick date and time slot");

        Doctor* doc = nullptr;
        for (int i = 0; i < doctors.size(); i++)
        {
            if (doctors.data[i].getId() == tmpDoctorId)
            {
                doc = &doctors.data[i];
                break;
            }
        }
        if (doc && fontOK)
        {
            char dbuf[128];
            snprintf(dbuf, 128, "Doctor: %s  |  Fee: PKR %.0f", doc->getName(), doc->getFees());
            drawText(win, font, dbuf, 24.f, H * 0.24f, 15, MC::GREEN);
        }

        inp[0].rect = { W * 0.05f, H * 0.32f, W * 0.38f, 44.f };
        inp[0].draw(win, font);

        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;

        if (slotCount > 0 && fontOK)
        {
            drawText(win, font, "Available Slots:", W * 0.50f, H * 0.30f, 15, MC::SUBTEXT);
            float sx = W * 0.50f, sy = H * 0.36f;
            float sw = 78.f, sh = 34.f, sgap = 8.f;
            for (int i = 0; i < slotCount; i++)
            {
                float bx = sx + (sw + sgap) * (i % 4);
                float by2 = sy + (sh + 6.f) * (i / 4);
                bool sel = (i == selectedSlot);
                bool hov = (mx >= bx && mx <= bx + sw && my >= by2 && my <= by2 + sh);
                drawRect(win, { bx, by2, sw, sh }, sel ? MC::ACCENT : (hov ? MC::CARD_HOV : MC::CARD), sel ? MC::ACCENT : MC::BORDER, 1.f);
                drawText(win, font, availableSlots[i], bx, by2 + sh / 2.f - 9, 14, sel ? MC::WHITE : MC::TEXT, true, sw);
            }
        }

        for (int i = 0; i < btnCount; i++)
        {
            btns[i].draw(win, font, mx, my);
        }
    }

    void drawWithListAndInput(const char* title)
    {
        drawPageHeader(title, "");
        list.rect = { W * 0.05f, H * 0.24f, W * 0.90f, H * 0.40f };
        list.draw(win, font);
        inp[0].rect = { W * 0.20f, H * 0.68f, W * 0.60f, 44.f };
        inp[0].draw(win, font);
        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;
        for (int i = 0; i < btnCount; i++)
        {
            btns[i].draw(win, font, mx, my);
        }
    }

    void drawFormScreen(const char* title)
    {
        drawPageHeader(title, "");
        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;
        for (int i = 0; i < 6; i++)
        {
            if (inp[i].label[0])
            {
                inp[i].draw(win, font);
            }
        }
        for (int i = 0; i < btnCount; i++)
        {
            btns[i].draw(win, font, mx, my);
        }
    }

    void drawListOnly(const char* title)
    {
        drawPageHeader(title, "");
        list.rect = { W * 0.05f, H * 0.24f, W * 0.90f, H * 0.64f };
        list.draw(win, font);
        sf::Vector2i mp = sf::Mouse::getPosition(win);
        float mx = (float)mp.x, my = (float)mp.y;
        for (int i = 0; i < btnCount; i++)
        {
            btns[i].draw(win, font, mx, my);
        }
    }

    void drawPageHeader(const char* title, const char* subtitle)
    {
        drawRect(win, { 0, 0, W, H }, MC::BG);
        if (fontOK)
        {
            sf::Text t(font, "MEDICORE", 32);
            t.setFillColor(MC::TITLE);
            t.setStyle(sf::Text::Italic);
            t.setPosition({ 22.f, 14.f });
            win.draw(t);
            drawText(win, font, title, 22.f, 56.f, 20, MC::TEXT);
            if (subtitle[0])
            {
                drawText(win, font, subtitle, 22.f, 82.f, 13, MC::SUBTEXT);
            }
        }
    }

    void handleEvent(const sf::Event& ev)
    {
        if (auto* mb = ev.getIf<sf::Event::MouseButtonPressed>())
        {
            float mx = (float)mb->position.x, my = (float)mb->position.y;
            handleClick(mx, my);
            for (int i = 0; i < 6; i++)
            {
                inp[i].checkClick(mx, my);
            }
        }
        for (int i = 0; i < 6; i++)
        {
            inp[i].handleEvent(ev);
        }
        list.handleScroll(ev);
        if (auto* kp = ev.getIf<sf::Event::KeyPressed>())
        {
            if (kp->code == sf::Keyboard::Key::Enter && btnCount > 0)
            {
                handleButtonAction(btns[0].id);
            }
        }
    }

    void handleClick(float mx, float my)
    {
        if (screen == Screen::ROLE_SELECT)
        {
            float cardX = 18.f, cardW = W - 36.f;
            float cardH = 60.f, cardGap = 10.f;
            float startY = 160.f;
            for (int i = 0; i < 4; i++)
            {
                float by = startY + i * (cardH + cardGap);
                if (mx >= cardX && mx <= cardX + cardW && my >= by && my <= by + cardH)
                {
                    if (i == 3)
                    {
                        win.close();
                        return;
                    }
                    goLogin(i + 1);
                    return;
                }
            }
            return;
        }

        if (screen == Screen::BOOK_STEP2 && slotCount > 0)
        {
            float sx = W * 0.50f, sy = H * 0.36f;
            float sw = 78.f, sh = 34.f, sgap = 8.f;
            for (int i = 0; i < slotCount; i++)
            {
                float bx = sx + (sw + sgap) * (i % 4);
                float by2 = sy + (sh + 6.f) * (i / 4);
                if (mx >= bx && mx <= bx + sw && my >= by2 && my <= by2 + sh)
                {
                    selectedSlot = i;
                    scopy(tmpSlot, availableSlots[i], 6);
                    return;
                }
            }
        }

        if (screen == Screen::BOOK_STEP1)
        {
            Rect lr = { W * 0.05f, H * 0.50f, W * 0.90f, H * 0.38f };
            if (lr.contains(mx, my) && doctorResultCount > 0)
            {
                int row = (int)((my - lr.y) / list.rowH);
                int idx = list.scroll + row;
                if (idx >= 0 && idx < doctorResultCount)
                {
                    selectedDoctorIdx = idx;
                    tmpDoctorId = doctorResults[idx];
                    return;
                }
            }
        }

        for (int i = 0; i < btnCount; i++)
        {
            if (btns[i].rect.contains(mx, my))
            {
                handleButtonAction(btns[i].id);
                return;
            }
        }
    }

    void handleButtonAction(int id)
    {
        switch (screen)
        {
        case Screen::LOGIN:
            handleLoginAction(id);
            break;
        case Screen::PATIENT_MENU:
            handlePatientMenu(id);
            break;
        case Screen::DOCTOR_MENU:
            handleDoctorMenu(id);
            break;
        case Screen::ADMIN_MENU:
            handleAdminMenu(id);
            break;
        case Screen::BOOK_STEP1:
            handleBookStep1(id);
            break;
        case Screen::BOOK_STEP2:
            handleBookStep2(id);
            break;
        case Screen::CANCEL_APPT:
            handleCancelAppt(id);
            break;
        case Screen::PAY_BILL:
            handlePayBill(id);
            break;
        case Screen::TOPUP:
            handleTopup(id);
            break;
        case Screen::MARK_COMPLETE:
            handleMarkComplete(id);
            break;
        case Screen::MARK_NOSHOW:
            handleMarkNoShow(id);
            break;
        case Screen::WRITE_PRESC:
            handleWritePresc(id);
            break;
        case Screen::VIEW_PAT_HIST:
            handleViewPatHist(id);
            break;
        case Screen::ADD_DOCTOR:
            handleAddDoctor(id);
            break;
        case Screen::REMOVE_DOCTOR:
            handleRemoveDoctor(id);
            break;
        case Screen::DISCHARGE:
            handleDischarge(id);
            break;
        case Screen::VIEW_APPTS:
        case Screen::VIEW_RECORDS:
        case Screen::VIEW_BILLS:
        case Screen::TODAY_APPTS:
        case Screen::VIEW_ALL_PATIENTS:
        case Screen::VIEW_ALL_DOCTORS:
        case Screen::VIEW_ALL_APPTS:
        case Screen::VIEW_UNPAID:
        case Screen::VIEW_SECLOG:
        case Screen::DAILY_REPORT:
            if (id == 99)
            {
                goBack();
            }
            break;
        default:
            if (id == 99)
            {
                goBack();
            }
            break;
        case Screen::ADD_PATIENT:
            handleAddPatient(id);
            break;
        case Screen::REMOVE_PATIENT:
            handleRemovePatient(id);
            break;
        }
    }

    void handleLoginAction(int id)
    {
        if (id == 1)
        {
            goRoleSelect();
            return;
        }
        char idStr[32];
        scopy(idStr, inp[0].buf, 32);
        char pw[64];
        scopy(pw, inp[1].buf, 64);
        int uid = 0;
        for (int i = 0; idStr[i]; i++)
        {
            if (idStr[i] >= '0' && idStr[i] <= '9')
            {
                uid = uid * 10 + (idStr[i] - '0');
            }
        }

        if (loginRole == 1)
        {
            Patient* p = patients.findById(uid);
            if (p && smatch(p->getPassword(), pw))
            {
                curPatient = p;
                curDoctor = nullptr;
                notif.show("Login successful!", false);
                goPatientMenu();
            }
            else
            {
                loginAttempts++;
                if (loginAttempts >= 3)
                {
                    notif.show("Account locked.", true);
                    goRoleSelect();
                }
                else
                {
                    char m[80];
                    snprintf(m, 80, "Invalid credentials. %d attempt(s) left.", 3 - loginAttempts);
                    notif.show(m, true);
                }
            }
        }
        else if (loginRole == 2)
        {
            Doctor* d = doctors.findById(uid);
            if (d && smatch(d->getPassword(), pw))
            {
                curDoctor = d;
                curPatient = nullptr;
                notif.show("Login successful!", false);
                goDoctorMenu();
            }
            else
            {
                loginAttempts++;
                if (loginAttempts >= 3)
                {
                    notif.show("Account locked.", true);
                    goRoleSelect();
                }
                else
                {
                    char m[80];
                    snprintf(m, 80, "Invalid credentials. %d attempt(s) left.", 3 - loginAttempts);
                    notif.show(m, true);
                }
            }
        }
        else if (loginRole == 3)
        {
            Admin admin(0, "Admin", "admin123");
            try
            {
                fh.load_admin(admin);
            }
            catch (...)
            {
            }
            if (admin.getId() == uid && smatch(admin.getPassword(), pw))
            {
                curPatient = nullptr;
                curDoctor = nullptr;
                notif.show("Admin login successful!", false);
                goAdminMenu();
            }
            else
            {
                loginAttempts++;
                if (loginAttempts >= 3)
                {
                    notif.show("Account locked.", true);
                    goRoleSelect();
                }
                else
                {
                    char m[80];
                    snprintf(m, 80, "Invalid credentials. %d attempt(s) left.", 3 - loginAttempts);
                    notif.show(m, true);
                }
            }
        }
    }

    void handlePatientMenu(int id)
    {
        switch (id)
        {
        case 0:
            goBookStep1();
            break;
        case 1:
            goCancelAppt();
            break;
        case 2:
            goViewAppts();
            break;
        case 3:
            goViewRecords();
            break;
        case 4:
            goViewBills();
            break;
        case 5:
            goPayBill();
            break;
        case 6:
            goTopup();
            break;
        case 7:
            curPatient = nullptr;
            goRoleSelect();
            break;
        }
    }

    void goBookStep1()
    {
        screen = Screen::BOOK_STEP1;
        btnCount = 0;
        clearInputs();
        setupInput(0, "Specialization", false, false);
        addBtn(0, "Search", { W * 0.62f, H * 0.30f, W * 0.18f, 40 }, MC::ACCENT);
        addBtn(1, "Select Doctor", { W * 0.20f, H * 0.91f, W * 0.30f, 38 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.60f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
        list.clear();
        doctorResultCount = 0;
        selectedDoctorIdx = -1;
    }

    void handleBookStep1(int id)
    {
        if (id == 99)
        {
            goPatientMenu();
            return;
        }
        if (id == 0)
        {
            list.clear();
            doctorResultCount = 0;
            selectedDoctorIdx = -1;
            char spec[51];
            scopy(spec, inp[0].buf, 51);
            for (int i = 0; spec[i]; i++)
            {
                if (spec[i] >= 'A' && spec[i] <= 'Z')
                {
                    spec[i] += 32;
                }
            }
            for (int i = 0; i < doctors.size(); i++)
            {
                char ds[51];
                const char* raw = doctors.data[i].getSpecialization();
                int j = 0;
                while (raw[j])
                {
                    ds[j] = (raw[j] >= 'A' && raw[j] <= 'Z') ? raw[j] + 32 : raw[j];
                    j++;
                }
                ds[j] = '\0';
                if (smatch(ds, spec))
                {
                    char row[256];
                    snprintf(row, 256, "ID: %-4d  %-25s  PKR %.0f", doctors.data[i].getId(), doctors.data[i].getName(), doctors.data[i].getFees());
                    list.addLine(row);
                    if (doctorResultCount < 64)
                    {
                        doctorResults[doctorResultCount++] = doctors.data[i].getId();
                    }
                }
            }
            if (list.count == 0)
            {
                notif.show("No doctors found.", true);
            }
        }
        if (id == 1)
        {
            if (selectedDoctorIdx < 0 || selectedDoctorIdx >= doctorResultCount)
            {
                notif.show("Select a doctor first.", true);
                return;
            }
            tmpDoctorId = doctorResults[selectedDoctorIdx];
            goBookStep2();
        }
    }



    void goBookStep2()
    {
        screen = Screen::BOOK_STEP2;
        btnCount = 0;
        clearInputs();
        setupInput(0, "Date (DD-MM-YYYY)", false, false);
        addBtn(0, "Check Slots", { W * 0.05f, H * 0.44f, W * 0.22f, 38 }, MC::ACCENT);
        addBtn(1, "Confirm Booking", { W * 0.05f, H * 0.88f, W * 0.35f, 44 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.50f, H * 0.88f, W * 0.18f, 44 }, MC::SUBTEXT);
        slotCount = 0;
        selectedSlot = -1;
    }



    void handleBookStep2(int id)
    {
        if (id == 99)
        {
            goBookStep1();
            return;
        }
        if (id == 0)
        {
            scopy(tmpDate, inp[0].buf, 11);
            if (!validator.validateDate(tmpDate))
            {
                notif.show("Inalid date. Use DD-MM-YYYY.", true);
                return;
            }
            const char* allSlots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
            slotCount = 0;
            selectedSlot = -1;
            for (int i = 0; i < 8; i++)
            {
                bool taken = false;
                for (int j = 0; j < appointments.size(); j++)
                {
                    if (appointments.data[j].doc_id == tmpDoctorId &&
                        smatch(appointments.data[j].date, tmpDate) &&
                        smatch(appointments.data[j].timeslot, allSlots[i]) &&
                        !smatch(appointments.data[j].status, "cancelled"))
                    {
                        taken = true;
                        break;
                    }
                }
                if (!taken)
                {
                    scopy(availableSlots[slotCount++], allSlots[i], 6);
                }
            }
            if (slotCount == 0)
            {
                notif.show("No slots available.", true);
            }
            else
            {
                notif.show("Slots loaded. Click to select.", false);
            }
        }
        if (id == 1)
        {
            if (selectedSlot < 0)
            {
                notif.show("Select a slot first.", true);
                return;
            }
            Doctor* doc = nullptr;
            for (int i = 0; i < doctors.size(); i++)
            {
                if (doctors.data[i].getId() == tmpDoctorId)
                {
                    doc = &doctors.data[i];
                    break;
                }
            }
            if (!doc)
            {
                notif.show("Doctor not found.", true);
                return;
            }
            if (curPatient->getBalance() < doc->getFees())
            {
                notif.show("Insufficient balance.", true);
                return;
            }
            *curPatient -= doc->getFees();

            int newApptId = 0;
            for (int i = 0; i < appointments.size(); i++)
            {
                if (appointments.data[i].appointment_id > newApptId)
                {
                    newApptId = appointments.data[i].appointment_id;
                }
            }
            newApptId++;
            Appointment a(newApptId, tmpDoctorId, curPatient->getId(), tmpDate, tmpSlot, "pending");
            appointments.add(a);
            fh.append_appointment(a);
            char today[11];
            get_current_date(today);
            int newBillId = 0;
            for (int i = 0; i < bills.size(); i++)
            {
                if (bills.data[i].bill_id > newBillId)
                {
                    newBillId = bills.data[i].bill_id;
                }
            }
            newBillId++;
            Bill b(newBillId, newApptId, curPatient->getId(), today, doc->getFees(), "unpaid");
            bills.add(b);
            fh.append_bill(b);
            fh.update_patients(patients);
            char msg[128];
            snprintf(msg, 128, "Appointment booked! ID: %d", newApptId);
            notif.show(msg, false);
            goPatientMenu();
        }
    }

    void goCancelAppt()
    {
        screen = Screen::CANCEL_APPT;
        btnCount = 0;
        clearInputs();
        list.clear();
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].patient_id == curPatient->getId() && smatch(appointments.data[i].status, "pending"))
            {
                Doctor* d = doctors.findById(appointments.data[i].doc_id);
                char row[256];
                snprintf(row, 256, "ID: %-4d  Dr.%-20s  %s  %s", appointments.data[i].appointment_id, d ? d->getName() : "Unknown", appointments.data[i].date, appointments.data[i].timeslot);
                list.addLine(row);
            }
        }
        if (list.count == 0)
        {
            list.addLine("No pending appointments.");
        }
        setupInput(0, "Appointment ID to cancel", false, false);
        addBtn(0, "Cancel Appointment", { W * 0.15f, H * 0.82f, W * 0.35f, 44 }, MC::RED);
        addBtn(99, "Back", { W * 0.58f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleCancelAppt(int id)
    {
        if (id == 99)
        {
            goPatientMenu();
            return;
        }
        int aid = char_to_int(inp[0].buf);
        Appointment* appt = appointments.findById(aid);
        if (!appt || appt->patient_id != curPatient->getId() || !smatch(appt->status, "pending"))
        {
            notif.show("Invalid appointment ID.", true);
            return;
        }
        scopy(appt->status, "cancelled", 10);
        fh.update_appointments(appointments);

        for (int i = 0; i < bills.size(); i++)
        {
            if (bills.data[i].appointment_id == aid)
            {
                if (smatch(bills.data[i].status, "paid"))
                {
                    *curPatient += bills.data[i].amount;
                    fh.update_patients(patients);
                    char msg[128];
                    snprintf(msg, 128, "Cancelled. PKR %.0f refunded.", bills.data[i].amount);
                    notif.show(msg, false);
                }
                else
                {
                    notif.show("Appointment cancelled.", false);
                }
                scopy(bills.data[i].status, "cancelled", 10);
                fh.update_bills(bills);
                break;
            }
        }
        goCancelAppt();
    }

    void goViewAppts()
    {
        screen = Screen::VIEW_APPTS;
        btnCount = 0;
        list.clear();
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].patient_id == curPatient->getId())
            {
                Doctor* d = doctors.findById(appointments.data[i].doc_id);
                char row[256];
                snprintf(row, 256, "ID:%-4d Dr.%-18s %s %s [%s]", appointments.data[i].appointment_id, d ? d->getName() : "Unknown", appointments.data[i].date, appointments.data[i].timeslot, appointments.data[i].status);
                list.addLine(row);
            }
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goViewRecords()
    {
        screen = Screen::VIEW_RECORDS;
        btnCount = 0;
        list.clear();
        for (int i = 0; i < prescriptions.size(); i++)
        {
            if (prescriptions.data[i].patient_id == curPatient->getId())
            {
                Doctor* d = doctors.findById(prescriptions.data[i].doctor_id);
                char row[256];
                snprintf(row, 256, "%s  Dr.%-15s  %s", prescriptions.data[i].date, d ? d->getName() : "Unknown", prescriptions.data[i].medicines);
                list.addLine(row);
                char row2[256];
                snprintf(row2, 256, "   Notes: %s", prescriptions.data[i].notes);
                list.addLine(row2);
            }
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goViewBills()
    {
        screen = Screen::VIEW_BILLS;
        btnCount = 0;
        list.clear();
        double unpaid = 0;
        for (int i = 0; i < bills.size(); i++)
        {
            if (bills.data[i].patient_id == curPatient->getId())
            {
                char row[256];
                snprintf(row, 256, "Bill#%-4d  Appt#%-4d  PKR %-8.0f  %-12s  %s", bills.data[i].bill_id, bills.data[i].appointment_id, bills.data[i].amount, bills.data[i].status, bills.data[i].date);
                list.addLine(row);
                if (smatch(bills.data[i].status, "unpaid"))
                {
                    unpaid += bills.data[i].amount;
                }
            }
        }
        char total[64];
        snprintf(total, 64, "-- Total Outstanding: PKR %.0f --", unpaid);
        list.addLine(total);
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goPayBill()
    {
        screen = Screen::PAY_BILL;
        btnCount = 0;
        clearInputs();
        list.clear();
        for (int i = 0; i < bills.size(); i++)
        {
            if (bills.data[i].patient_id == curPatient->getId() && smatch(bills.data[i].status, "unpaid"))
            {
                char row[256];
                snprintf(row, 256, "Bill#%-4d  PKR %.0f  Date: %s", bills.data[i].bill_id, bills.data[i].amount, bills.data[i].date);
                list.addLine(row);
            }
        }
        setupInput(0, "Bill ID to pay", false, false);
        addBtn(0, "Pay Bill", { W * 0.15f, H * 0.82f, W * 0.30f, 44 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.55f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handlePayBill(int id)
    {
        if (id == 99)
        {
            goPatientMenu();
            return;
        }
        int bid = char_to_int(inp[0].buf);
        Bill* bill = bills.findById(bid);
        if (!bill || bill->patient_id != curPatient->getId() || !smatch(bill->status, "unpaid"))
        {
            notif.show("Invalid bill ID.", true);
            return;
        }
        if (curPatient->getBalance() < bill->amount)
        {
            notif.show("Insufficient balance.", true);
            return;
        }
        *curPatient -= bill->amount;
        scopy(bill->status, "paid", 10);
        fh.update_bills(bills);
        fh.update_patients(patients);
        char msg[128];
        snprintf(msg, 128, "Bill paid. Balance: PKR %.0f", curPatient->getBalance());
        notif.show(msg, false);
        goPayBill();
    }

    void goTopup()
    {
        screen = Screen::TOPUP;
        btnCount = 0;
        clearInputs();
        setupInput(0, "Amount to add (PKR)", false, false);
        inp[0].rect = { W * 0.25f, H * 0.40f, W * 0.50f, 44.f };
        addBtn(0, "Add Funds", { W * 0.25f, H * 0.56f, W * 0.28f, 46 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.58f, H * 0.56f, W * 0.18f, 46 }, MC::SUBTEXT);
    }

    void handleTopup(int id)
    {
        if (id == 99)
        {
            goPatientMenu();
            return;
        }
        double amount = 0;
        const char* buf = inp[0].buf;
        bool hasDot = false;
        double dec = 0.1;
        bool afterDot = false;
        for (int i = 0; buf[i]; i++)
        {
            if (buf[i] == '.' && !hasDot)
            {
                hasDot = true;
                afterDot = true;
            }
            else if (buf[i] >= '0' && buf[i] <= '9')
            {
                if (!afterDot)
                {
                    amount = amount * 10 + (buf[i] - '0');
                }
                else
                {
                    amount += dec * (buf[i] - '0');
                    dec *= 0.1;
                }
            }
        }
        if (amount <= 0)
        {
            notif.show("Enter a valid amount.", true);
            return;
        }
        *curPatient += amount;
        fh.update_patients(patients);
        char msg[128];
        snprintf(msg, 128, "Balance updated: PKR %.0f", curPatient->getBalance());
        notif.show(msg, false);
        goPatientMenu();
    }

    void handleDoctorMenu(int id)
    {
        switch (id)
        {
        case 0:
            goTodayAppts();
            break;
        case 1:
            goMarkComplete();
            break;
        case 2:
            goMarkNoShow();
            break;
        case 3:
            goWritePresc();
            break;
        case 4:
            goViewPatHist();
            break;
        case 5:
            curDoctor = nullptr;
            goRoleSelect();
            break;
        }
    }

    void goTodayAppts()
    {
        screen = Screen::TODAY_APPTS;
        btnCount = 0;
        list.clear();
        char today[11];
        get_current_date(today);
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].doc_id == curDoctor->getId() && smatch(appointments.data[i].date, today))
            {
                Patient* p = patients.findById(appointments.data[i].patient_id);
                char row[256];
                snprintf(row, 256, "ID:%-4d %-20s %s [%s]", appointments.data[i].appointment_id, p ? p->getName() : "Unknown", appointments.data[i].timeslot, appointments.data[i].status);
                list.addLine(row);
            }
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goMarkComplete()
    {
        screen = Screen::MARK_COMPLETE;
        btnCount = 0;
        clearInputs();
        list.clear();
        char today[11];
        get_current_date(today);
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].doc_id == curDoctor->getId() && smatch(appointments.data[i].date, today) && smatch(appointments.data[i].status, "pending"))
            {
                Patient* p = patients.findById(appointments.data[i].patient_id);
                char row[256];
                snprintf(row, 256, "ID:%-4d %-20s %s", appointments.data[i].appointment_id, p ? p->getName() : "Unknown", appointments.data[i].timeslot);
                list.addLine(row);
            }
        }
        setupInput(0, "Appointment ID", false, false);
        addBtn(0, "Mark Complete", { W * 0.15f, H * 0.82f, W * 0.30f, 44 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.55f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleMarkComplete(int id)
    {
        if (id == 99)
        {
            goDoctorMenu();
            return;
        }
        char today[11];
        get_current_date(today);
        int aid = char_to_int(inp[0].buf);
        Appointment* appt = appointments.findById(aid);
        if (!appt || appt->doc_id != curDoctor->getId() || !smatch(appt->status, "pending") || !smatch(appt->date, today))
        {
            notif.show("Invalid appointment ID.", true);
            return;
        }
        scopy(appt->status, "completed", 10);
        fh.update_appointments(appointments);
        notif.show("Marked as completed.", false);
        goMarkComplete();
    }

    void goMarkNoShow()
    {
        screen = Screen::MARK_NOSHOW;
        btnCount = 0;
        clearInputs();
        list.clear();
        char today[11];
        get_current_date(today);
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].doc_id == curDoctor->getId() && smatch(appointments.data[i].date, today) && smatch(appointments.data[i].status, "pending"))
            {
                Patient* p = patients.findById(appointments.data[i].patient_id);
                char row[256];
                snprintf(row, 256, "ID:%-4d %-20s %s", appointments.data[i].appointment_id, p ? p->getName() : "Unknown", appointments.data[i].timeslot);
                list.addLine(row);
            }
        }
        setupInput(0, "Appointment ID", false, false);
        addBtn(0, "Mark NoShow", { W * 0.15f, H * 0.82f, W * 0.30f, 44 }, MC::ACCENT);
        addBtn(99, "Back", { W * 0.55f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleMarkNoShow(int id)
    {
        if (id == 99)
        {
            goDoctorMenu();
            return;
        }
        char today[11];
        get_current_date(today);
        int aid = char_to_int(inp[0].buf);
        Appointment* appt = appointments.findById(aid);
        if (!appt || appt->doc_id != curDoctor->getId() || !smatch(appt->status, "pending") || !smatch(appt->date, today))
        {
            notif.show("Invalid appointment ID.", true);
            return;
        }
        scopy(appt->status, "no-show", 10);
        fh.update_appointments(appointments);
        for (int i = 0; i < bills.size(); i++)
        {
            if (bills.data[i].appointment_id == aid)
            {
                scopy(bills.data[i].status, "cancelled", 10);
                break;
            }
        }
        fh.update_bills(bills);
        notif.show("Marked as no show.", false);
        goMarkNoShow();
    }

    void goWritePresc()
    {
        screen = Screen::WRITE_PRESC;
        btnCount = 0;
        clearInputs();
        setupInput(0, "Appointment ID (completed)", false, false);
        inp[0].rect = { W * 0.05f, H * 0.26f, W * 0.42f, 44.f };
        setupInput(1, "Medicines (e.g. Paracetamol 500mg; Amoxicillin)", false, false);
        inp[1].rect = { W * 0.05f, H * 0.44f, W * 0.90f, 44.f };
        inp[1].maxLen = 250;
        setupInput(2, "Notes", false, false);
        inp[2].rect = { W * 0.05f, H * 0.60f, W * 0.90f, 44.f };
        inp[2].maxLen = 150;
        addBtn(0, "Save Prescription", { W * 0.20f, H * 0.78f, W * 0.35f, 46 }, MC::ACCENT);
        addBtn(99, "Back", { W * 0.62f, H * 0.78f, W * 0.18f, 46 }, MC::SUBTEXT);
    }

    void handleWritePresc(int id)
    {
        if (id == 99)
        {
            goDoctorMenu();
            return;
        }
        int aid = char_to_int(inp[0].buf);
        Appointment* appt = appointments.findById(aid);
        if (!appt || appt->doc_id != curDoctor->getId() || !smatch(appt->status, "completed"))
        {
            notif.show("Invalid or incomplete appointment.", true);
            return;
        }
        for (int i = 0; i < prescriptions.size(); i++)
        {
            if (prescriptions.data[i].appointment_id == aid)
            {
                notif.show("Prescription already exists.", true);
                return;
            }
        }
        char today[11];
        get_current_date(today);
        int newId = 0;
        for (int i = 0; i < prescriptions.size(); i++)
        {
            if (prescriptions.data[i].prescription_id > newId)
            {
                newId = prescriptions.data[i].prescription_id;
            }
        }
        newId++;
        Prescription pres(newId, aid, appt->patient_id, curDoctor->getId(), today, inp[1].buf, inp[2].buf);
        prescriptions.add(pres);
        fh.append_prescription(pres);
        notif.show("Prescription saved.", false);
        goDoctorMenu();
    }

    void goViewPatHist()
    {
        screen = Screen::VIEW_PAT_HIST;
        btnCount = 0;
        clearInputs();
        list.clear();
        setupInput(0, "Patient ID", false, false);
        addBtn(0, "Load History", { W * 0.20f, H * 0.68f, W * 0.30f, 44 }, MC::ACCENT);
        addBtn(99, "Back", { W * 0.58f, H * 0.68f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleViewPatHist(int id)
    {
        if (id == 99)
        {
            goDoctorMenu();
            return;
        }
        int pid = char_to_int(inp[0].buf);
        Patient* p = patients.findById(pid);
        if (!p)
        {
            notif.show("Patient not found.", true);
            return;
        }
        bool hasSeen = false;
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].patient_id == pid && appointments.data[i].doc_id == curDoctor->getId() && smatch(appointments.data[i].status, "completed"))
            {
                hasSeen = true;
                break;
            }
        }
        if (!hasSeen)
        {
            notif.show("No completed appointments with this patient.", true);
            return;
        }
        list.clear();
        for (int i = 0; i < prescriptions.size(); i++)
        {
            if (prescriptions.data[i].patient_id == pid && prescriptions.data[i].doctor_id == curDoctor->getId())
            {
                char row[256];
                snprintf(row, 256, "%s  %s", prescriptions.data[i].date, prescriptions.data[i].medicines);
                list.addLine(row);
                char row2[256];
                snprintf(row2, 256, "   Notes: %s", prescriptions.data[i].notes);
                list.addLine(row2);
            }
        }
        if (list.count == 0)
        {
            list.addLine("No prescriptions for this patient.");
        }
    }

    void handleAdminMenu(int id)
    {
        switch (id)
        {
        case 0:
            goAddDoctor();
            break;
        case 1:
            goRemoveDoctor();
            break;
        case 2:
            goViewAllPatients();
            break;
        case 3:
            goViewAllDoctors();
            break;
        case 4:
            goViewAllAppts();
            break;
        case 5:
            goViewUnpaid();
            break;
        case 6:
            goDischarge();
            break;
        case 7:
            goSecLog();
            break;
        case 8:
            goDailyReport();
            break;
        case 9:
            goAddPatient();
            break;
        case 10:
            goRemovePatient();
            break;
        case 11:
            goRoleSelect();
            break;
        }
    }

    void goAddDoctor()
    {
        screen = Screen::ADD_DOCTOR;
        btnCount = 0;
        clearInputs();
        scopy(inp[0].label, "Name", 64);
        inp[0].rect = { W * 0.05f, H * 0.22f, W * 0.42f, 40.f };
        inp[0].maxLen = 50;
        scopy(inp[1].label, "Specialization", 64);
        inp[1].rect = { W * 0.53f, H * 0.22f, W * 0.42f, 40.f };
        inp[1].maxLen = 50;
        scopy(inp[2].label, "Contact (11 digits)", 64);
        inp[2].rect = { W * 0.05f, H * 0.40f, W * 0.42f, 40.f };
        inp[2].maxLen = 12;
        scopy(inp[3].label, "Password (min 6 chars)", 64);
        inp[3].rect = { W * 0.53f, H * 0.40f, W * 0.42f, 40.f };
        inp[3].password = true;
        inp[3].maxLen = 50;
        scopy(inp[4].label, "Consultation Fee (PKR)", 64);
        inp[4].rect = { W * 0.05f, H * 0.58f, W * 0.42f, 40.f };
        inp[4].maxLen = 20;
        addBtn(0, "Add Doctor", { W * 0.25f, H * 0.76f, W * 0.30f, 46 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.62f, H * 0.76f, W * 0.18f, 46 }, MC::SUBTEXT);
    }

    void handleAddDoctor(int id)
    {
        if (id == 99)
        {
            goAdminMenu();
            return;
        }
        if (!validator.validateContact(inp[2].buf))
        {
            notif.show("Invalid contact (11 digits).", true);
            return;
        }
        if (!validator.validatePassword(inp[3].buf))
        {
            notif.show("Password too short (min 6).", true);
            return;
        }
        double fee = 0;
        const char* fb = inp[4].buf;
        for (int i = 0; fb[i]; i++)
        {
            if (fb[i] >= '0' && fb[i] <= '9')
            {
                fee = fee * 10 + (fb[i] - '0');
            }
        }
        if (fee <= 0)
        {
            notif.show("Invalid fee.", true);
            return;
        }
        int maxId = 0;
        for (int i = 0; i < doctors.size(); i++)
        {
            if (doctors.data[i].getId() > maxId)
            {
                maxId = doctors.data[i].getId();
            }
        }
        int newId = maxId + 1;
        Doctor d(newId, inp[0].buf, inp[1].buf, inp[3].buf, inp[2].buf, fee);
        doctors.add(d);
        fh.append_doctor(d);
        char msg[64];
        snprintf(msg, 64, "Doctor added. ID: %d", newId);
        notif.show(msg, false);
        goAdminMenu();
    }

    void goRemoveDoctor()
    {
        screen = Screen::REMOVE_DOCTOR;
        btnCount = 0;
        clearInputs();
        list.clear();
        for (int i = 0; i < doctors.size(); i++)
        {
            char row[256];
            snprintf(row, 256, "ID:%-4d %-25s %-20s PKR %.0f", doctors.data[i].getId(), doctors.data[i].getName(), doctors.data[i].getSpecialization(), doctors.data[i].getFees());
            list.addLine(row);
        }
        setupInput(0, "Doctor ID to remove", false, false);
        addBtn(0, "Remove Doctor", { W * 0.15f, H * 0.82f, W * 0.30f, 44 }, MC::RED);
        addBtn(99, "Back", { W * 0.55f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleRemoveDoctor(int id)
    {
        if (id == 99)
        {
            goAdminMenu();
            return;
        }
        int did = char_to_int(inp[0].buf);
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].doc_id == did && smatch(appointments.data[i].status, "pending"))
            {
                notif.show("Doctor has pending appointments.", true);
                return;
            }
        }
        doctors.removeById(did);
        fh.update_doctors(doctors);
        notif.show("Doctor removed.", false);
        goRemoveDoctor();
    }

    void goAddPatient()
    {
        screen = Screen::ADD_PATIENT;
        btnCount = 0;
        clearInputs();
        scopy(inp[0].label, "Name", 64);
        inp[0].rect = { W * 0.05f, H * 0.22f, W * 0.42f, 40.f };
        inp[0].maxLen = 50;
        scopy(inp[1].label, "Age", 64);
        inp[1].rect = { W * 0.53f, H * 0.22f, W * 0.42f, 40.f };
        inp[1].maxLen = 4;
        scopy(inp[2].label, "Gender (M/F)", 64);
        inp[2].rect = { W * 0.05f, H * 0.40f, W * 0.42f, 40.f };
        inp[2].maxLen = 2;
        scopy(inp[3].label, "Contact (11 digits)", 64);
        inp[3].rect = { W * 0.53f, H * 0.40f, W * 0.42f, 40.f };
        inp[3].maxLen = 12;
        scopy(inp[4].label, "Password (min 6 chars)", 64);
        inp[4].rect = { W * 0.05f, H * 0.58f, W * 0.42f, 40.f };
        inp[4].password = true;
        inp[4].maxLen = 50;
        addBtn(0, "Add Patient", { W * 0.25f, H * 0.76f, W * 0.30f, 46 }, MC::GREEN);
        addBtn(99, "Back", { W * 0.62f, H * 0.76f, W * 0.18f, 46 }, MC::SUBTEXT);
    }

    void handleAddPatient(int id)
    {
        if (id == 99)
        {
            goAdminMenu();
            return;
        }
        if (!validator.validateContact(inp[3].buf))
        {
            notif.show("Invalid contact (11 digits).", true);
            return;
        }
        if (!validator.validatePassword(inp[4].buf))
        {
            notif.show("Password too short (min 6).", true);
            return;
        }
        if (inp[0].buf[0] == '\0')
        {
            notif.show("Name cannot be empty.", true);
            return;
        }
        int age = char_to_int(inp[1].buf);
        if (age <= 0 || age > 150)
        {
            notif.show("Invalid age.", true);
            return;
        }
        bool gender = (inp[2].buf[0] == 'M' || inp[2].buf[0] == 'm');

        int maxId = 0;
        for (int i = 0; i < patients.size(); i++)
        {
            if (patients.data[i].getId() > maxId)
            {
                maxId = patients.data[i].getId();
            }
        }
        int newId = maxId + 1;

        Patient p(newId, inp[0].buf, age, gender, inp[4].buf, inp[3].buf, 0.0);
        patients.add(p);
        fh.append_patient(p);
        char msg[64];
        snprintf(msg, 64, "Patient added. ID: %d", newId);
        notif.show(msg, false);
        goAdminMenu();
    }
    // new func to make 
    void goRemovePatient()
    {
        screen = Screen::REMOVE_PATIENT;
        btnCount = 0;
        clearInputs();
        list.clear();
        for (int i = 0; i < patients.size(); i++)
        {
            char row[256];
            snprintf(row, 256, "ID:%-4d %-20s Age:%-3d PKR %.0f", patients.data[i].getId(), patients.data[i].getName(), patients.data[i].getAge(), patients.data[i].getBalance());
            list.addLine(row);
        }
        setupInput(0, "Patient ID to remove", false, false);
        addBtn(0, "Remove Patient", { W * 0.15f, H * 0.82f, W * 0.30f, 44 }, MC::RED);
        addBtn(99, "Back", { W * 0.55f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleRemovePatient(int id)
    {
        if (id == 99)
        {
            goAdminMenu();
            return;
        }
        int pid = char_to_int(inp[0].buf);
        Patient* p = patients.findById(pid);
        if (!p)
        {
            notif.show("Patient not found.", true);
            return;
        }
        for (int i = 0; i < bills.size(); i++)
        {
            if (bills.data[i].patient_id == pid && smatch(bills.data[i].status, "unpaid"))
            {
                notif.show("Patient has unpaid bills.", true);
                return;
            }
        }
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].patient_id == pid && smatch(appointments.data[i].status, "pending"))
            {
                notif.show("Patient has pending appointments.", true);
                return;
            }
        }
        patients.removeById(pid);
        fh.update_patients(patients);
        notif.show("Patient removed.", false);
        goRemovePatient();
    }

    void goViewAllPatients()
    {
        screen = Screen::VIEW_ALL_PATIENTS;
        btnCount = 0;
        list.clear();
        for (int i = 0; i < patients.size(); i++)
        {
            int unpaid = 0;
            for (int j = 0; j < bills.size(); j++)
            {
                if (bills.data[j].patient_id == patients.data[i].getId() && smatch(bills.data[j].status, "unpaid"))
                {
                    unpaid++;
                }
            }
            char row[256];
            snprintf(row, 256, "ID:%-4d %-20s Age:%-3d PKR %-8.0f Unpaid:%d", patients.data[i].getId(), patients.data[i].getName(), patients.data[i].getAge(), patients.data[i].getBalance(), unpaid);
            list.addLine(row);
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goViewAllDoctors()
    {
        screen = Screen::VIEW_ALL_DOCTORS;
        btnCount = 0;
        list.clear();
        for (int i = 0; i < doctors.size(); i++)
        {
            char row[256];
            snprintf(row, 256, "ID:%-4d %-20s %-20s PKR %.0f", doctors.data[i].getId(), doctors.data[i].getName(), doctors.data[i].getSpecialization(), doctors.data[i].getFees());
            list.addLine(row);
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goViewAllAppts()
    {
        screen = Screen::VIEW_ALL_APPTS;
        btnCount = 0;
        list.clear();
        for (int i = 0; i < appointments.size(); i++)
        {
            Patient* p = patients.findById(appointments.data[i].patient_id);
            Doctor* d = doctors.findById(appointments.data[i].doc_id);
            char row[256];
            snprintf(row, 256, "ID:%-4d Pat:%-15s Dr:%-15s %s %s [%s]", appointments.data[i].appointment_id, p ? p->getName() : "Unknown", d ? d->getName() : "Unknown", appointments.data[i].date, appointments.data[i].timeslot, appointments.data[i].status);
            list.addLine(row);
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goViewUnpaid()
    {
        screen = Screen::VIEW_UNPAID;
        btnCount = 0;
        list.clear();
        for (int i = 0; i < bills.size(); i++)
        {
            if (smatch(bills.data[i].status, "unpaid"))
            {
                Patient* p = patients.findById(bills.data[i].patient_id);
                char row[256];
                snprintf(row, 256, "Bill#%-4d %-20s PKR %-8.0f %s", bills.data[i].bill_id, p ? p->getName() : "Unknown", bills.data[i].amount, bills.data[i].date);
                list.addLine(row);
            }
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goDischarge()
    {
        screen = Screen::DISCHARGE;
        btnCount = 0;
        clearInputs();
        list.clear();
        for (int i = 0; i < patients.size(); i++)
        {
            char row[256];
            snprintf(row, 256, "ID:%-4d %-20s PKR %.0f", patients.data[i].getId(), patients.data[i].getName(), patients.data[i].getBalance());
            list.addLine(row);
        }
        setupInput(0, "Patient ID to discharge", false, false);
        addBtn(0, "Discharge", { W * 0.15f, H * 0.82f, W * 0.28f, 44 }, MC::RED);
        addBtn(99, "Back", { W * 0.55f, H * 0.82f, W * 0.18f, 44 }, MC::SUBTEXT);
    }

    void handleDischarge(int id)
    {
        if (id == 99)
        {
            goAdminMenu();
            return;
        }
        int pid = char_to_int(inp[0].buf);
        Patient* p = patients.findById(pid);
        if (!p)
        {
            notif.show("Patient not found.", true);
            return;
        }
        for (int i = 0; i < bills.size(); i++)
        {
            if (bills.data[i].patient_id == pid && smatch(bills.data[i].status, "unpaid"))
            {
                notif.show("Patient has unpaid bills.", true);
                return;
            }
        }
        for (int i = 0; i < appointments.size(); i++)
        {
            if (appointments.data[i].patient_id == pid && smatch(appointments.data[i].status, "pending"))
            {
                notif.show("Patient has pending appointments.", true);
                return;
            }
        }
        fh.append_discharged(*p);
        patients.removeById(pid);
        fh.update_patients(patients);
        notif.show("Patient discharged.", false);
        goDischarge();
    }

    void goSecLog()
    {
        screen = Screen::VIEW_SECLOG;
        btnCount = 0;
        list.clear();
        FILE* f = fopen("security_log.txt", "r");
        if (f)
        {
            char line[256];
            while (fgets(line, 256, f))
            {
                int n = slen(line);
                if (n > 0 && line[n - 1] == '\n')
                {
                    line[n - 1] = '\0';
                }
                list.addLine(line);
            }
            fclose(f);
        }
        if (list.count == 0)
        {
            list.addLine("No security events logged.");
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goDailyReport()
    {
        screen = Screen::DAILY_REPORT;
        btnCount = 0;
        list.clear();
        char today[11];
        get_current_date(today);
        int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
        double revenue = 0;
        for (int i = 0; i < appointments.size(); i++)
        {
            if (smatch(appointments.data[i].date, today))
            {
                total++;
                if (smatch(appointments.data[i].status, "pending"))
                {
                    pending++;
                }
                else if (smatch(appointments.data[i].status, "completed"))
                {
                    completed++;
                }
                else if (smatch(appointments.data[i].status, "no-show"))
                {
                    noshow++;
                }
                else if (smatch(appointments.data[i].status, "cancelled"))
                {
                    cancelled++;
                }
            }
        }
        for (int i = 0; i < bills.size(); i++)
        {
            if (smatch(bills.data[i].status, "paid") && smatch(bills.data[i].date, today))
            {
                revenue += bills.data[i].amount;
            }
        }
        char row[256];
        snprintf(row, 256, "Date: %s", today);
        list.addLine(row);
        snprintf(row, 256, "Appointments: %d  (Pend:%d Done:%d No-show:%d Cancel:%d)", total, pending, completed, noshow, cancelled);
        list.addLine(row);
        snprintf(row, 256, "Revenue Today: PKR %.0f", revenue);
        list.addLine(row);
        list.addLine("------------------------------------------------------");
        list.addLine("Patients with Unpaid Bills:");
        for (int i = 0; i < patients.size(); i++)
        {
            double owed = 0;
            for (int j = 0; j < bills.size(); j++)
            {
                if (bills.data[j].patient_id == patients.data[i].getId() && smatch(bills.data[j].status, "unpaid"))
                {
                    owed += bills.data[j].amount;
                }
            }
            if (owed > 0)
            {
                snprintf(row, 256, "  %-20s  PKR %.0f", patients.data[i].getName(), owed);
                list.addLine(row);
            }
        }
        list.addLine("------------------------------------------------------");
        list.addLine("Doctor Summary:");
        for (int i = 0; i < doctors.size(); i++)
        {
            int dc = 0, dp = 0, dn = 0;
            for (int j = 0; j < appointments.size(); j++)
            {
                if (appointments.data[j].doc_id == doctors.data[i].getId() && smatch(appointments.data[j].date, today))
                {
                    if (smatch(appointments.data[j].status, "completed"))
                    {
                        dc++;
                    }
                    else if (smatch(appointments.data[j].status, "pending"))
                    {
                        dp++;
                    }
                    else if (smatch(appointments.data[j].status, "no-show"))
                    {
                        dn++;
                    }
                }
            }
            snprintf(row, 256, "  Dr.%-20s Done:%d Pend:%d No-show:%d", doctors.data[i].getName(), dc, dp, dn);
            list.addLine(row);
        }
        addBtn(99, "Back", { W * 0.40f, H * 0.91f, W * 0.20f, 38 }, MC::SUBTEXT);
    }

    void goBack()
    {
        if (curPatient)
        {
            goPatientMenu();
        }
        else if (curDoctor)
        {
            goDoctorMenu();
        }
        else
        {
            goAdminMenu();
        }
    }

    void clearInputs()
    {
        for (int i = 0; i < 6; i++)
        {
            inp[i].buf[0] = '\0';
            inp[i].label[0] = '\0';
            inp[i].focused = false;
            inp[i].password = false;
            inp[i].maxLen = 50;
        }
    }

    void setupInput(int idx, const char* lbl, bool focus, bool pw)
    {
        scopy(inp[idx].label, lbl, 64);
        inp[idx].focused = focus;
        inp[idx].password = pw;
        inp[idx].buf[0] = '\0';
    }

    void addBtn(int id, const char* lbl, Rect r, sf::Color col)
    {
        if (btnCount >= 16)
        {
            return;
        }
        btns[btnCount].id = id;
        scopy(btns[btnCount].label, lbl, 64);
        btns[btnCount].rect = r;
        btns[btnCount].col = col;
        btnCount++;
    }

    void layoutMenuCards(const char** labels, sf::Color* cols, int n)
    {
        float cardW = W * 0.44f, cardH = 44.f, gap = 10.f;
        float startX = W * 0.04f, startY = H * 0.23f;
        for (int i = 0; i < n; i++)
        {
            int col = i % 2, row = i / 2;
            float bx = startX + col * (cardW + W * 0.08f);
            float by = startY + row * (cardH + gap);
            addBtn(i, labels[i], { bx, by, cardW, cardH }, cols[i]);
        }
    }

    static int char_to_int(const char* s)
    {
        int r = 0;
        for (int i = 0; s[i]; i++)
        {
            if (s[i] >= '0' && s[i] <= '9')
            {
                r = r * 10 + (s[i] - '0');
            }
        }
        return r;
    }
};

inline void ui()
{
    MediCoreUI app;
    app.run();
}