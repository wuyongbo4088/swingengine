// Swing Engine Version 1 Source Code 
// Most of techniques in the engine are mainly based on David Eberly's
// Wild Magic 4 open-source code.The author of Swing Engine learned a lot
// from Eberly's experience of architecture and algorithm.
// Several sub-systems are totally new,and others are re-implimented or
// re-organized based on Wild Magic 4's sub-systems.
// Copyright (c) 2007-2010.  All Rights Reserved
//
// Eberly's permission:
// Geometric Tools, Inc.
// http://www.geometrictools.com
// Copyright (c) 1998-2006.  All Rights Reserved
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.  The license is available for reading at
// the location:
// http://www.gnu.org/copyleft/lgpl.html

#pragma once

using namespace System::Runtime::InteropServices;

extern ImageExporterApp g_ImageExporterApp;

namespace SEImageExporter_VC80
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Form1 ժҪ
	///
	/// ����: ������Ĵ�������ƣ�����Ҫ����
	///          ����������������� .resx �ļ��������й���Դ���������ߵ�
	///          ����Դ�ļ��������ԡ�����
	///          �������������˴���Ĺ���
	///          ���ػ���Դ��ȷ������
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: �ڴ˴���ӹ��캯������
			//
		}

	protected:
		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::PictureBox^  SceneWindow;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	protected: 

	private:
		/// <summary>
		/// ����������������
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// �����֧������ķ��� - ��Ҫ
		/// ʹ�ô���༭���޸Ĵ˷��������ݡ�
		/// </summary>
		void InitializeComponent(void)
		{
			this->SceneWindow = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SceneWindow))->BeginInit();
			this->SuspendLayout();
			// 
			// SceneWindow
			// 
			this->SceneWindow->Location = System::Drawing::Point(12, 12);
			this->SceneWindow->Name = L"SceneWindow";
			this->SceneWindow->Size = System::Drawing::Size(640, 480);
			this->SceneWindow->TabIndex = 0;
			this->SceneWindow->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(665, 38);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Open File";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(665, 78);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Save As";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(752, 516);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->SceneWindow);
			this->Name = L"Form1";
			this->Text = L"ImageExportor";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->SceneWindow))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 
		System::Void button1_Click(System::Object^, System::EventArgs^)
		{
			OpenFileDialog^ pDialog = gcnew OpenFileDialog;
			pDialog->InitialDirectory = "D:\\";
			pDialog->Filter = "jpg|*.jpg|tga|*.tga|bmp|*.bmp|dds|*.dds";
			pDialog->FilterIndex = 1;
			pDialog->RestoreDirectory = true;
			pDialog->ShowDialog();

			// ��System::String����ת��Ϊcharָ��.
			char* acFilename = (char*)(void*)Marshal::StringToHGlobalAnsi(pDialog->FileName);

			size_t iLen = strlen(acFilename);
			if( strncmp(acFilename, "", iLen) != 0 )
			{
				g_ImageExporterApp.OnOpenFile(acFilename);
			}
		}

		System::Void button2_Click(System::Object^, System::EventArgs^)
		{
			SaveFileDialog^ pDialog = gcnew SaveFileDialog;
			pDialog->InitialDirectory = "D:\\";
			pDialog->Filter = "seif|*.seif";
			pDialog->FilterIndex = 1;
			pDialog->RestoreDirectory = true;
			pDialog->ShowDialog();

			// ��System::String����ת��Ϊcharָ��.
			char* acFilename = (char*)(void*)Marshal::StringToHGlobalAnsi(pDialog->FileName);

			size_t iLen = strlen(acFilename);
			if( strncmp(acFilename, "", iLen) != 0 )
			{
				g_ImageExporterApp.OnSave(acFilename);
			}
		}
};
}

