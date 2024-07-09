namespace csharp_test_client
{
    partial class mainForm
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

		#region Windows Form 디자이너에서 생성한 코드

		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다. 
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
		/// </summary>
		private void InitializeComponent()
		{
			btnDisconnect = new System.Windows.Forms.Button();
			btnConnect = new System.Windows.Forms.Button();
			groupBox5 = new System.Windows.Forms.GroupBox();
			textBoxPort = new System.Windows.Forms.TextBox();
			label10 = new System.Windows.Forms.Label();
			checkBoxLocalHostIP = new System.Windows.Forms.CheckBox();
			textBoxIP = new System.Windows.Forms.TextBox();
			label9 = new System.Windows.Forms.Label();
			button1 = new System.Windows.Forms.Button();
			textSendText = new System.Windows.Forms.TextBox();
			labelStatus = new System.Windows.Forms.Label();
			listBoxLog = new System.Windows.Forms.ListBox();
			label1 = new System.Windows.Forms.Label();
			textBoxUserID = new System.Windows.Forms.TextBox();
			textBoxUserPW = new System.Windows.Forms.TextBox();
			label2 = new System.Windows.Forms.Label();
			button2 = new System.Windows.Forms.Button();
			Room = new System.Windows.Forms.GroupBox();
			roomListReqBtn = new System.Windows.Forms.Button();
			roomListText = new System.Windows.Forms.Label();
			textBoxRelay = new System.Windows.Forms.TextBox();
			btnRoomRelay = new System.Windows.Forms.Button();
			btnRoomChat = new System.Windows.Forms.Button();
			textBoxRoomSendMsg = new System.Windows.Forms.TextBox();
			listBoxRoomChatMsg = new System.Windows.Forms.ListBox();
			label4 = new System.Windows.Forms.Label();
			listBoxRoomUserList = new System.Windows.Forms.ListBox();
			listBoxRoomList = new System.Windows.Forms.ListBox();
			btn_RoomLeave = new System.Windows.Forms.Button();
			btn_RoomEnter = new System.Windows.Forms.Button();
			textBoxRoomNumber = new System.Windows.Forms.TextBox();
			label3 = new System.Windows.Forms.Label();
			roomTitleLabel = new System.Windows.Forms.Label();
			curLobbyLabel = new System.Windows.Forms.Label();
			curLobbyLabelNum = new System.Windows.Forms.Label();
			roomTitleText = new System.Windows.Forms.TextBox();
			groupBox1 = new System.Windows.Forms.GroupBox();
			lobbyLeaveBtn = new System.Windows.Forms.Button();
			lobbyEnterButton = new System.Windows.Forms.Button();
			lobbyListReqBtn = new System.Windows.Forms.Button();
			listBoxLobby = new System.Windows.Forms.ListBox();
			groupBox5.SuspendLayout();
			Room.SuspendLayout();
			groupBox1.SuspendLayout();
			SuspendLayout();
			// 
			// btnDisconnect
			// 
			btnDisconnect.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			btnDisconnect.Location = new System.Drawing.Point(601, 92);
			btnDisconnect.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			btnDisconnect.Name = "btnDisconnect";
			btnDisconnect.Size = new System.Drawing.Size(126, 54);
			btnDisconnect.TabIndex = 29;
			btnDisconnect.Text = "접속 끊기";
			btnDisconnect.UseVisualStyleBackColor = true;
			btnDisconnect.Click += btnDisconnect_Click;
			// 
			// btnConnect
			// 
			btnConnect.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			btnConnect.Location = new System.Drawing.Point(600, 33);
			btnConnect.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			btnConnect.Name = "btnConnect";
			btnConnect.Size = new System.Drawing.Size(126, 54);
			btnConnect.TabIndex = 28;
			btnConnect.Text = "접속하기";
			btnConnect.UseVisualStyleBackColor = true;
			btnConnect.Click += btnConnect_Click;
			// 
			// groupBox5
			// 
			groupBox5.Controls.Add(textBoxPort);
			groupBox5.Controls.Add(label10);
			groupBox5.Controls.Add(checkBoxLocalHostIP);
			groupBox5.Controls.Add(textBoxIP);
			groupBox5.Controls.Add(label9);
			groupBox5.Location = new System.Drawing.Point(17, 25);
			groupBox5.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			groupBox5.Name = "groupBox5";
			groupBox5.Padding = new System.Windows.Forms.Padding(4, 6, 4, 6);
			groupBox5.Size = new System.Drawing.Size(576, 108);
			groupBox5.TabIndex = 27;
			groupBox5.TabStop = false;
			groupBox5.Text = "Socket 더미 클라이언트 설정";
			// 
			// textBoxPort
			// 
			textBoxPort.Location = new System.Drawing.Point(321, 42);
			textBoxPort.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxPort.MaxLength = 6;
			textBoxPort.Name = "textBoxPort";
			textBoxPort.Size = new System.Drawing.Size(71, 31);
			textBoxPort.TabIndex = 18;
			textBoxPort.Text = "32452";
			textBoxPort.WordWrap = false;
			// 
			// label10
			// 
			label10.AutoSize = true;
			label10.Location = new System.Drawing.Point(233, 50);
			label10.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			label10.Name = "label10";
			label10.Size = new System.Drawing.Size(94, 25);
			label10.TabIndex = 17;
			label10.Text = "포트 번호:";
			// 
			// checkBoxLocalHostIP
			// 
			checkBoxLocalHostIP.AutoSize = true;
			checkBoxLocalHostIP.Checked = true;
			checkBoxLocalHostIP.CheckState = System.Windows.Forms.CheckState.Checked;
			checkBoxLocalHostIP.Location = new System.Drawing.Point(407, 50);
			checkBoxLocalHostIP.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			checkBoxLocalHostIP.Name = "checkBoxLocalHostIP";
			checkBoxLocalHostIP.Size = new System.Drawing.Size(152, 29);
			checkBoxLocalHostIP.TabIndex = 15;
			checkBoxLocalHostIP.Text = "localhost 사용";
			checkBoxLocalHostIP.UseVisualStyleBackColor = true;
			// 
			// textBoxIP
			// 
			textBoxIP.Location = new System.Drawing.Point(97, 40);
			textBoxIP.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxIP.MaxLength = 6;
			textBoxIP.Name = "textBoxIP";
			textBoxIP.Size = new System.Drawing.Size(123, 31);
			textBoxIP.TabIndex = 11;
			textBoxIP.Text = "0.0.0.0";
			textBoxIP.WordWrap = false;
			// 
			// label9
			// 
			label9.AutoSize = true;
			label9.Location = new System.Drawing.Point(9, 48);
			label9.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			label9.Name = "label9";
			label9.Size = new System.Drawing.Size(94, 25);
			label9.TabIndex = 10;
			label9.Text = "서버 주소:";
			// 
			// button1
			// 
			button1.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			button1.Location = new System.Drawing.Point(456, 146);
			button1.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			button1.Name = "button1";
			button1.Size = new System.Drawing.Size(143, 54);
			button1.TabIndex = 39;
			button1.Text = "echo 보내기";
			button1.UseVisualStyleBackColor = true;
			button1.Click += button1_Click;
			// 
			// textSendText
			// 
			textSendText.Location = new System.Drawing.Point(17, 154);
			textSendText.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textSendText.MaxLength = 32;
			textSendText.Name = "textSendText";
			textSendText.Size = new System.Drawing.Size(428, 31);
			textSendText.TabIndex = 38;
			textSendText.Text = "test1";
			textSendText.WordWrap = false;
			// 
			// labelStatus
			// 
			labelStatus.AutoSize = true;
			labelStatus.Location = new System.Drawing.Point(14, 1410);
			labelStatus.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			labelStatus.Name = "labelStatus";
			labelStatus.Size = new System.Drawing.Size(166, 25);
			labelStatus.TabIndex = 40;
			labelStatus.Text = "서버 접속 상태: ???";
			// 
			// listBoxLog
			// 
			listBoxLog.FormattingEnabled = true;
			listBoxLog.HorizontalScrollbar = true;
			listBoxLog.ItemHeight = 25;
			listBoxLog.Location = new System.Drawing.Point(14, 1100);
			listBoxLog.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			listBoxLog.Name = "listBoxLog";
			listBoxLog.Size = new System.Drawing.Size(698, 279);
			listBoxLog.TabIndex = 41;
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(14, 235);
			label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(71, 25);
			label1.TabIndex = 42;
			label1.Text = "UserID:";
			// 
			// textBoxUserID
			// 
			textBoxUserID.Location = new System.Drawing.Point(97, 229);
			textBoxUserID.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxUserID.MaxLength = 6;
			textBoxUserID.Name = "textBoxUserID";
			textBoxUserID.Size = new System.Drawing.Size(123, 31);
			textBoxUserID.TabIndex = 43;
			textBoxUserID.Text = "jacking75";
			textBoxUserID.WordWrap = false;
			// 
			// textBoxUserPW
			// 
			textBoxUserPW.Location = new System.Drawing.Point(97, 279);
			textBoxUserPW.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxUserPW.MaxLength = 6;
			textBoxUserPW.Name = "textBoxUserPW";
			textBoxUserPW.Size = new System.Drawing.Size(123, 31);
			textBoxUserPW.TabIndex = 45;
			textBoxUserPW.Text = "jacking75";
			textBoxUserPW.WordWrap = false;
			// 
			// label2
			// 
			label2.AutoSize = true;
			label2.Location = new System.Drawing.Point(14, 290);
			label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size(81, 25);
			label2.TabIndex = 44;
			label2.Text = "PassWD:";
			// 
			// button2
			// 
			button2.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			button2.Location = new System.Drawing.Point(14, 335);
			button2.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			button2.Name = "button2";
			button2.Size = new System.Drawing.Size(207, 54);
			button2.TabIndex = 46;
			button2.Text = "Login";
			button2.UseVisualStyleBackColor = true;
			button2.Click += login_Click;
			// 
			// Room
			// 
			Room.Controls.Add(roomListReqBtn);
			Room.Controls.Add(roomListText);
			Room.Controls.Add(textBoxRelay);
			Room.Controls.Add(btnRoomRelay);
			Room.Controls.Add(btnRoomChat);
			Room.Controls.Add(textBoxRoomSendMsg);
			Room.Controls.Add(listBoxRoomChatMsg);
			Room.Controls.Add(label4);
			Room.Controls.Add(listBoxRoomUserList);
			Room.Controls.Add(listBoxRoomList);
			Room.Controls.Add(btn_RoomLeave);
			Room.Controls.Add(btn_RoomEnter);
			Room.Controls.Add(textBoxRoomNumber);
			Room.Controls.Add(roomTitleLabel);
			Room.Controls.Add(roomTitleText);

			Room.Controls.Add(label3);
			Room.Location = new System.Drawing.Point(19, 531);
			Room.Margin = new System.Windows.Forms.Padding(4);
			Room.Name = "Room";
			Room.Padding = new System.Windows.Forms.Padding(4);
			Room.Size = new System.Drawing.Size(707, 521);
			Room.TabIndex = 47;
			Room.TabStop = false;
			Room.Text = "Room";
			 
			// button6
			// 
			roomListReqBtn.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			roomListReqBtn.Location = new System.Drawing.Point(405, 77);
			roomListReqBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			roomListReqBtn.Name = "button6";
			roomListReqBtn.Size = new System.Drawing.Size(126, 54);
			roomListReqBtn.TabIndex = 58;
			roomListReqBtn.Text = "Room List";
			roomListReqBtn.UseVisualStyleBackColor = true;
			roomListReqBtn.Click += btnRoomListReq_Click;

			// roomListText
			// 
			roomListText.AutoSize = true;
			roomListText.Location = new System.Drawing.Point(14, 36);
			roomListText.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			roomListText.Name = "roomListText";
			roomListText.Size = new System.Drawing.Size(98, 25);
			roomListText.TabIndex = 57;
			roomListText.Text = "Room List:";
			// 
			// textBoxRelay
			// 
			textBoxRelay.Location = new System.Drawing.Point(598, 36);
			textBoxRelay.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxRelay.MaxLength = 6;
			textBoxRelay.Name = "textBoxRelay";
			textBoxRelay.Size = new System.Drawing.Size(94, 31);
			textBoxRelay.TabIndex = 55;
			textBoxRelay.Text = "test";
			textBoxRelay.WordWrap = false;
			// 
			// btnRoomRelay
			// 
			btnRoomRelay.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			btnRoomRelay.Location = new System.Drawing.Point(599, 77);
			btnRoomRelay.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			btnRoomRelay.Name = "btnRoomRelay";
			btnRoomRelay.Size = new System.Drawing.Size(94, 54);
			btnRoomRelay.TabIndex = 54;
			btnRoomRelay.Text = "Relay";
			btnRoomRelay.UseVisualStyleBackColor = true;
			btnRoomRelay.Click += btnRoomRelay_Click;
			// 
			// btnRoomChat
			// 
			btnRoomChat.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			btnRoomChat.Location = new System.Drawing.Point(624, 442);
			btnRoomChat.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			btnRoomChat.Name = "btnRoomChat";
			btnRoomChat.Size = new System.Drawing.Size(71, 54);
			btnRoomChat.TabIndex = 53;
			btnRoomChat.Text = "chat";
			btnRoomChat.UseVisualStyleBackColor = true;
			btnRoomChat.Click += btnRoomChat_Click;
			// 
			// textBoxRoomSendMsg
			// 
			textBoxRoomSendMsg.Location = new System.Drawing.Point(19, 448);
			textBoxRoomSendMsg.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxRoomSendMsg.MaxLength = 32;
			textBoxRoomSendMsg.Name = "textBoxRoomSendMsg";
			textBoxRoomSendMsg.Size = new System.Drawing.Size(597, 31);
			textBoxRoomSendMsg.TabIndex = 52;
			textBoxRoomSendMsg.Text = "test1";
			textBoxRoomSendMsg.WordWrap = false;
			// 
			// listBoxRoomChatMsg
			// 
			listBoxRoomChatMsg.FormattingEnabled = true;
			listBoxRoomChatMsg.ItemHeight = 25;
			listBoxRoomChatMsg.Location = new System.Drawing.Point(207, 200);
			listBoxRoomChatMsg.Margin = new System.Windows.Forms.Padding(4);
			listBoxRoomChatMsg.Name = "listBoxRoomChatMsg";
			listBoxRoomChatMsg.Size = new System.Drawing.Size(488, 229);
			listBoxRoomChatMsg.TabIndex = 51;
			// 
			// label4
			// 
			label4.AutoSize = true;
			label4.Location = new System.Drawing.Point(14, 171);
			label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			label4.Name = "label4";
			label4.Size = new System.Drawing.Size(86, 25);
			label4.TabIndex = 50;
			label4.Text = "User List:";
			// 
			// listBoxRoomUserList
			// 
			listBoxRoomUserList.FormattingEnabled = true;
			listBoxRoomUserList.ItemHeight = 25;
			listBoxRoomUserList.Location = new System.Drawing.Point(14, 200);
			listBoxRoomUserList.Margin = new System.Windows.Forms.Padding(4);
			listBoxRoomUserList.Name = "listBoxRoomUserList";
			listBoxRoomUserList.Size = new System.Drawing.Size(174, 229);
			listBoxRoomUserList.TabIndex = 49;
			// 
			// listBoxRoomList
			// 
			listBoxRoomList.ItemHeight = 25;
			listBoxRoomList.Location = new System.Drawing.Point(14, 64);
			listBoxRoomList.Name = "listBoxRoomList";
			listBoxRoomList.Size = new System.Drawing.Size(174, 104);
			listBoxRoomList.TabIndex = 56;
			// 
			// btn_RoomLeave
			// 
			btn_RoomLeave.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			btn_RoomLeave.Location = new System.Drawing.Point(309, 77);
			btn_RoomLeave.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			btn_RoomLeave.Name = "btn_RoomLeave";
			btn_RoomLeave.Size = new System.Drawing.Size(94, 54);
			btn_RoomLeave.TabIndex = 48;
			btn_RoomLeave.Text = "Leave";
			btn_RoomLeave.UseVisualStyleBackColor = true;
			btn_RoomLeave.Click += btn_RoomLeave_Click;
			// 
			// btn_RoomEnter
			// 
			btn_RoomEnter.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			btn_RoomEnter.Location = new System.Drawing.Point(207, 77);
			btn_RoomEnter.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			btn_RoomEnter.Name = "btn_RoomEnter";
			btn_RoomEnter.Size = new System.Drawing.Size(94, 54);
			btn_RoomEnter.TabIndex = 47;
			btn_RoomEnter.Text = "Enter";
			btn_RoomEnter.UseVisualStyleBackColor = true;
			btn_RoomEnter.Click += btn_RoomEnter_Click;

			// 
			// label3
			// 
			label3.AutoSize = true;
			label3.Location = new System.Drawing.Point(206, 36);
			label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			label3.Name = "label3";
			label3.Size = new System.Drawing.Size(138, 25);
			label3.TabIndex = 43;
			label3.Text = "Room Num:";

			// 
			// textBoxRoomNumber
			// 
			textBoxRoomNumber.Location = new System.Drawing.Point(322, 36);
			textBoxRoomNumber.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			textBoxRoomNumber.MaxLength = 6;
			textBoxRoomNumber.Name = "textBoxRoomNumber";
			textBoxRoomNumber.Size = new System.Drawing.Size(33, 31);
			textBoxRoomNumber.TabIndex = 44;
			textBoxRoomNumber.Text = "0";
			textBoxRoomNumber.WordWrap = false;

			// 
			// roomTitleLabel
			// 
			roomTitleLabel.AutoSize = true;
			roomTitleLabel.Location = new System.Drawing.Point(360, 36);
			roomTitleLabel.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			roomTitleLabel.Name = "roomTitleLabel";
			roomTitleLabel.Size = new System.Drawing.Size(138, 25);
			roomTitleLabel.TabIndex = 43;
			roomTitleLabel.Text = "New Title";

			// 
			// roomTitleText
			// 
			roomTitleText.Location = new System.Drawing.Point(450, 36);
			roomTitleText.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			roomTitleText.MaxLength = 6;
			roomTitleText.Name = "textBoxRoomNumber";
			roomTitleText.Size = new System.Drawing.Size(60, 31);
			roomTitleText.TabIndex = 44;
			roomTitleText.Text = "";
			roomTitleText.WordWrap = false;

			// 
			// curLobbyLabel
			// 
			curLobbyLabel.AutoSize = true;
			curLobbyLabel.Location = new System.Drawing.Point(327, 244);
			curLobbyLabel.Name = "curLobbyLabel";
			curLobbyLabel.Size = new System.Drawing.Size(119, 25);
			curLobbyLabel.TabIndex = 54;
			curLobbyLabel.Text = "Lobby Num :";
			// 
			// curLobbyLabelNum
			// 
			curLobbyLabelNum.AutoSize = true;
			curLobbyLabelNum.Location = new System.Drawing.Point(452, 244);
			curLobbyLabelNum.Name = "curLobbyLabelNum";
			curLobbyLabelNum.Size = new System.Drawing.Size(0, 25);
			curLobbyLabelNum.TabIndex = 54;
			// 
			// groupBox1
			// 
			groupBox1.Controls.Add(lobbyLeaveBtn);
			groupBox1.Controls.Add(lobbyEnterButton);
			groupBox1.Controls.Add(lobbyListReqBtn);
			groupBox1.Controls.Add(listBoxLobby);
			groupBox1.Controls.Add(curLobbyLabel);
			groupBox1.Controls.Add(curLobbyLabelNum);
			groupBox1.Location = new System.Drawing.Point(230, 229);
			groupBox1.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			groupBox1.Name = "groupBox1";
			groupBox1.Padding = new System.Windows.Forms.Padding(4, 6, 4, 6);
			groupBox1.Size = new System.Drawing.Size(496, 292);
			groupBox1.TabIndex = 48;
			groupBox1.TabStop = false;
			groupBox1.Text = "Lobby";
			// 
			// button5
			// 
			lobbyLeaveBtn.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			lobbyLeaveBtn.Location = new System.Drawing.Point(330, 175);
			lobbyLeaveBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			lobbyLeaveBtn.Name = "button5";
			lobbyLeaveBtn.Size = new System.Drawing.Size(153, 54);
			lobbyLeaveBtn.TabIndex = 53;
			lobbyLeaveBtn.Text = "Leave";
			lobbyLeaveBtn.UseVisualStyleBackColor = true;
			lobbyLeaveBtn.Click += btnLobbyLeaveReq_Click;
			// 
			// button4
			// 
			lobbyEnterButton.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			lobbyEnterButton.Location = new System.Drawing.Point(330, 108);
			lobbyEnterButton.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			lobbyEnterButton.Name = "lobbyEnterButton";
			lobbyEnterButton.Size = new System.Drawing.Size(153, 54);
			lobbyEnterButton.TabIndex = 52;
			lobbyEnterButton.Text = "Enter";
			lobbyEnterButton.UseVisualStyleBackColor = true;
			lobbyEnterButton.Click += btnLobbyEnterReq_Click;
			// 
			// roomListReqBtn
			// 
			lobbyListReqBtn.Font = new System.Drawing.Font("맑은 고딕", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
			lobbyListReqBtn.Location = new System.Drawing.Point(330, 42);
			lobbyListReqBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			lobbyListReqBtn.Name = "roomListReqBtn";
			lobbyListReqBtn.Size = new System.Drawing.Size(153, 54);
			lobbyListReqBtn.TabIndex = 51;
			lobbyListReqBtn.Text = "Lobby List";
			lobbyListReqBtn.UseVisualStyleBackColor = true;
			lobbyListReqBtn.Click += buttonLobbyListReq_Click;
			// 
			// listBoxLobby
			// 
			listBoxLobby.FormattingEnabled = true;
			listBoxLobby.ItemHeight = 25;
			listBoxLobby.Location = new System.Drawing.Point(16, 40);
			listBoxLobby.Margin = new System.Windows.Forms.Padding(4);
			listBoxLobby.Name = "listBoxLobby";
			listBoxLobby.Size = new System.Drawing.Size(304, 229);
			listBoxLobby.TabIndex = 50;
			// 
			// mainForm
			// 
			AutoScaleDimensions = new System.Drawing.SizeF(10F, 25F);
			AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			ClientSize = new System.Drawing.Size(1028, 1500);
			Controls.Add(groupBox1);
			Controls.Add(Room);
			Controls.Add(button2);
			Controls.Add(textBoxUserPW);
			Controls.Add(label2);
			Controls.Add(textBoxUserID);
			Controls.Add(label1);
			Controls.Add(labelStatus);
			Controls.Add(listBoxLog);
			Controls.Add(button1);
			Controls.Add(textSendText);
			Controls.Add(btnDisconnect);
			Controls.Add(btnConnect);
			Controls.Add(groupBox5);
			FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
			Name = "mainForm";
			Text = "네트워크 테스트 클라이언트";
			FormClosing += mainForm_FormClosing;
			Load += mainForm_Load;
			groupBox5.ResumeLayout(false);
			groupBox5.PerformLayout();
			Room.ResumeLayout(false);
			Room.PerformLayout();
			groupBox1.ResumeLayout(false);
			groupBox1.PerformLayout();
			ResumeLayout(false);
			PerformLayout();
		}


		#endregion

		private System.Windows.Forms.Button btnDisconnect;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.TextBox textBoxPort;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.CheckBox checkBoxLocalHostIP;
        private System.Windows.Forms.TextBox textBoxIP;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textSendText;
        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.ListBox listBoxLog;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxUserID;
        private System.Windows.Forms.TextBox textBoxUserPW;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button2;

        private System.Windows.Forms.GroupBox Room;
        private System.Windows.Forms.Button btn_RoomLeave;
        private System.Windows.Forms.Button btn_RoomEnter;
        private System.Windows.Forms.TextBox textBoxRoomNumber;

        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label roomTitleLabel;
        private System.Windows.Forms.TextBox roomTitleText;
        private System.Windows.Forms.Label curLobbyLabel;
        private System.Windows.Forms.Label curLobbyLabelNum;
        private System.Windows.Forms.Button btnRoomChat;
        private System.Windows.Forms.TextBox textBoxRoomSendMsg;
        private System.Windows.Forms.ListBox listBoxRoomChatMsg;
		private System.Windows.Forms.ListBox listBoxRoomList;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ListBox listBoxRoomUserList;
        private System.Windows.Forms.Button btnRoomRelay;
        private System.Windows.Forms.TextBox textBoxRelay;
        private System.Windows.Forms.GroupBox groupBox1;

        private System.Windows.Forms.Button lobbyLeaveBtn;
        private System.Windows.Forms.Button lobbyEnterButton;
        private System.Windows.Forms.Button lobbyListReqBtn;
        private System.Windows.Forms.ListBox listBoxLobby;

		private System.Windows.Forms.Label	roomListText;
		private System.Windows.Forms.Button roomListReqBtn;
	}
}

