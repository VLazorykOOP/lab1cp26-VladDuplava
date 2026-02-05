#pragma once
#include <cmath>

namespace Project5task1 {
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    private:
        // Масив полів для зручного створення
        array<TextBox^>^ inputs;
        array<Label^>^ labels;
        Button^ btnDraw;

    public:
        Form1(void)
        {
            InitializeComponent();
            this->DoubleBuffered = true;
        }

    protected:
        ~Form1() { if (components) delete components; }

    private:
        System::ComponentModel::Container^ components;

        void InitializeComponent(void)
        {
            this->inputs = gcnew array<TextBox^>(8);
            this->labels = gcnew array<Label^>(8);
            array<String^>^ names = { "P1.X", "P1.Y", "P2.X", "P2.Y", "V1.X", "V1.Y", "V2.X", "V2.Y" };
            array<String^>^ values = { "100", "400", "600", "200", "200", "-600", "400", "500" };

            int startY = 20;
            for (int i = 0; i < 8; i++) {
                // Створення підписів
                labels[i] = gcnew Label();
                labels[i]->Text = names[i];
                labels[i]->Location = Point(10, startY);
                labels[i]->Size = System::Drawing::Size(40, 20);
                this->Controls->Add(labels[i]);

                // Створення полів введення
                inputs[i] = gcnew TextBox();
                inputs[i]->Text = values[i];
                inputs[i]->Location = Point(55, startY);
                inputs[i]->Size = System::Drawing::Size(60, 20);
                this->Controls->Add(inputs[i]);

                startY += 30;
                if (i == 3) startY += 15; // Відступ між точками та векторами
            }

            btnDraw = gcnew Button();
            btnDraw->Text = "Побудувати";
            btnDraw->Location = Point(10, startY);
            btnDraw->Size = System::Drawing::Size(105, 30);
            btnDraw->Click += gcnew EventHandler(this, &Form1::btnDraw_Click);
            this->Controls->Add(btnDraw);

            this->ClientSize = System::Drawing::Size(900, 650);
            this->Text = L"Інтерактивний сплайн Ерміта";
            this->Paint += gcnew PaintEventHandler(this, &Form1::Form1_Paint);
        }

        // Базові поліноми Ерміта згідно з вашими формулами
        double h00(double t) { return 2 * Math::Pow(t, 3) - 3 * Math::Pow(t, 2) + 1; }
        double h01(double t) { return -2 * Math::Pow(t, 3) + 3 * Math::Pow(t, 2); }
        double h10(double t) { return Math::Pow(t, 3) - 2 * Math::Pow(t, 2) + t; }
        double h11(double t) { return Math::Pow(t, 3) - Math::Pow(t, 2); }

        void btnDraw_Click(Object^ sender, EventArgs^ e) {
            this->Invalidate(); // Виклик перемалювання
        }

        void Form1_Paint(Object^ sender, PaintEventArgs^ e)
        {
            Graphics^ g = e->Graphics;
            g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
            g->Clear(Color::White);

            try {
                // Отримуємо значення з полів
                float px1 = float::Parse(inputs[0]->Text); float py1 = float::Parse(inputs[1]->Text);
                float px2 = float::Parse(inputs[2]->Text); float py2 = float::Parse(inputs[3]->Text);
                float vx1 = float::Parse(inputs[4]->Text); float vy1 = float::Parse(inputs[5]->Text);
                float vx2 = float::Parse(inputs[6]->Text); float vy2 = float::Parse(inputs[7]->Text);

                PointF p0(px1, py1), p1(px2, py2), v0(vx1, vy1), v1(vx2, vy2);

                // Побудова кривої як лінійної комбінації базових функцій
                Pen^ splinePen = gcnew Pen(Color::Orange, 3.0f);
                PointF prev = p0;
                for (int i = 1; i <= 100; i++) {
                    double t = (double)i / 100.0;
                    // p(t) = h00(t)p0 + h10(t)m0 + h01(t)p1 + h11(t)m1
                    float x = (float)(h00(t) * p0.X + h10(t) * v0.X + h01(t) * p1.X + h11(t) * v1.X);
                    float y = (float)(h00(t) * p0.Y + h10(t) * v0.Y + h01(t) * p1.Y + h11(t) * v1.Y);

                    PointF curr(x, y);
                    g->DrawLine(splinePen, prev, curr);
                    prev = curr;
                }

                // Малюємо точки (виправлено типи для FillEllipse)
                g->FillEllipse(Brushes::Red, p0.X - 5.0f, p0.Y - 5.0f, 10.0f, 10.0f);
                g->FillEllipse(Brushes::Blue, p1.X - 5.0f, p1.Y - 5.0f, 10.0f, 10.0f);

                // Візуалізація векторів напрямку
                Pen^ vPen = gcnew Pen(Color::LightGray, 1.0f);
                g->DrawLine(vPen, p0, PointF(p0.X + v0.X / 3.0f, p0.Y + v0.Y / 3.0f));
                g->DrawLine(vPen, p1, PointF(p1.X + v1.X / 3.0f, p1.Y + v1.Y / 3.0f));

            }
            catch (...) {
                // Якщо введено не число, нічого не малюємо
            }
        }
    };
}
