#pragma once

#include <cmath> // Для використання sin, cos

namespace Project5task2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			// Вмикаємо подвійну буферизацію для плавного малювання без мерехтіння
			this->DoubleBuffered = true;
		}

	protected:
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		// Оголошення елементів керування (вирішує помилку C2065)
		System::Windows::Forms::TrackBar^ trackA;
		System::Windows::Forms::TrackBar^ trackK;
		System::Windows::Forms::Label^ lblA;
		System::Windows::Forms::Label^ lblK;
		System::ComponentModel::Container^ components;

		void InitializeComponent(void)
		{
			this->trackA = (gcnew System::Windows::Forms::TrackBar());
			this->trackK = (gcnew System::Windows::Forms::TrackBar());
			this->lblA = (gcnew System::Windows::Forms::Label());
			this->lblK = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();

			// Налаштування повзунка А (кількість відрізків у пучку)
			this->trackA->Location = System::Drawing::Point(12, 30);
			this->trackA->Minimum = 1;
			this->trackA->Maximum = 12;
			this->trackA->Value = 4;
			this->trackA->Size = System::Drawing::Size(200, 45);
			this->trackA->Scroll += gcnew System::EventHandler(this, &MyForm::OnParametersChanged);

			// Налаштування повзунка К (щільність/кількість точок)
			this->trackK->Location = System::Drawing::Point(12, 90);
			this->trackK->Minimum = 1;
			this->trackK->Maximum = 100;
			this->trackK->Value = 20;
			this->trackK->Size = System::Drawing::Size(200, 45);
			this->trackK->Scroll += gcnew System::EventHandler(this, &MyForm::OnParametersChanged);

			this->lblA->Text = "Кількість гілок (A): " + trackA->Value;
			this->lblA->Location = System::Drawing::Point(12, 10);
			this->lblA->AutoSize = true;

			this->lblK->Text = "Якість (K * 1000): " + trackK->Value;
			this->lblK->Location = System::Drawing::Point(12, 75);
			this->lblK->AutoSize = true;

			// Параметри вікна
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::Black;
			this->ClientSize = System::Drawing::Size(800, 700);
			this->Controls->Add(this->lblK);
			this->Controls->Add(this->lblA);
			this->Controls->Add(this->trackK);
			this->Controls->Add(this->trackA);
			this->Name = L"MyForm";
			this->Text = L"Фрактальна Папороть - Пучок";
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);
			this->ResumeLayout(false);
			this->PerformLayout();
		}

		// Оновлення при зміні значень повзунків
		void OnParametersChanged(System::Object^ sender, System::EventArgs^ e) {
			this->lblA->Text = "Кількість гілок (A): " + trackA->Value;
			this->lblK->Text = "Якість (K * 1000): " + trackK->Value;
			this->Invalidate(); // Перемалювати форму
		}

		void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
			Graphics^ g = e->Graphics;
			// Використовуємо SmoothingMode для кращої якості
			g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

			int A = trackA->Value;
			int K = trackK->Value * 1000;
			Random^ rand = gcnew Random();

			// Масив кольорів для різних листків
			array<Color>^ colors = {
				Color::Lime, Color::Cyan, Color::Gold,
				Color::Magenta, Color::OrangeRed, Color::SpringGreen
			};

			for (int i = 0; i < A; i++) {
				double x = 0, y = 0;
				// Кут повороту для кожного листка у "пучку"
				double angle = (i * 2.0 * Math::PI) / A;
				SolidBrush^ brush = gcnew SolidBrush(colors[i % colors->Length]);

				for (int j = 0; j < K; j++) {
					double nextX, nextY;
					double r = rand->NextDouble();

					// Математичні перетворення Барнслі
					if (r < 0.01) {
						nextX = 0;
						nextY = 0.16 * y;
					}
					else if (r < 0.86) {
						nextX = 0.85 * x + 0.04 * y;
						nextY = -0.04 * x + 0.85 * y + 1.6;
					}
					else if (r < 0.93) {
						nextX = 0.2 * x - 0.26 * y;
						nextY = 0.23 * x + 0.22 * y + 1.6;
					}
					else {
						nextX = -0.15 * x + 0.28 * y;
						nextY = 0.26 * x + 0.24 * y + 0.44;
					}
					x = nextX;
					y = nextY;

					// Масштабування (scale) та поворот
					double scale = 50.0;
					// Центруємо фрактал (400, 450)
					int screenX = (int)(400 + (x * Math::Cos(angle) - y * Math::Sin(angle)) * scale);
					int screenY = (int)(450 - (x * Math::Sin(angle) + y * Math::Cos(angle)) * scale);

					// Малюємо точку, якщо вона в межах екрана
					if (screenX > 0 && screenX < 800 && screenY > 0 && screenY < 700) {
						g->FillRectangle(brush, screenX, screenY, 1, 1);
					}
				}
				delete brush; // Очищення пам'яті
			}
		}
	};
}
