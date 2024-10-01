#include "main.h"

#if defined(__aarch64__)
    #define GNativeAndroidApp_Offset 0xc7d2a08
#else
    #define GNativeAndroidApp_Offset 0x8eb6be8
#endif

//App credentials code
//Simply replace the placeholder code in the example with these
std::string name = "APP_NAME"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "OWNER_ID"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string sellerkey = "SELLER_KEY"; // seller key, the blurred text on seller setting tab
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = "https://licenseauth.host/api/1.2/"; //licenceauth is free auth service (free seller plan), use KeyAuth if you have a seller plan

struct sRegion
{
	uintptr_t start, end;
};
std::vector<sRegion> trapRegions;

using namespace ImGui;
namespace c
{

	inline ImVec4 accent = ImColor(255, 255, 0, 215);

	namespace background
	{

		inline ImVec4 filling = ImColor(12, 12, 12);
		inline ImVec4 stroke = ImColor(24, 26, 36);
		inline ImVec2 size = ImVec2(900, 515);

		inline float rounding = 6;

	}

	namespace elements
	{
		inline ImVec4 mark = ImColor(255, 255, 255);

		inline ImVec4 stroke = ImColor(28, 26, 37);
		inline ImVec4 background = ImColor(15, 15, 17);
		inline ImVec4 background_widget = ImColor(21, 23, 26);

		inline ImVec4 text_active = ImColor(255, 255, 255);
		inline ImVec4 text_hov = ImColor(81, 92, 109);
		inline ImVec4 text = ImColor(43, 51, 63);

		inline float rounding = 4;
	}

	namespace child
	{

	}

	namespace tab
	{
		inline ImVec4 tab_active = ImColor(22, 22, 30);

		inline ImVec4 border = ImColor(14, 14, 15);
	}

}
auto picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;
void CricleProgress(const char* name, float progress, float max, float radius, const ImVec2& size)
{
    const float tickness = 3.f;
    static float position = 0.f;

    position = progress / max * 6.28f;

    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcTo(GetCursorScreenPos() + size, radius, 0.f, 2.f * IM_PI, 120.f);
    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::elements::background_widget), 0, tickness);

    ImGui::GetForegroundDrawList()->PathClear();
    ImGui::GetForegroundDrawList()->PathArcTo(GetCursorScreenPos() + size, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 120.f);
    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::accent), 0, tickness);

    int procent = progress / (int)max * 100;

    std::string procent_str = std::to_string(procent) + "%";

}

struct Notification {
    int id;
    std::string message;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point endTime;
};

class NotificationSystem {
public:
    NotificationSystem() : notificationIdCounter(0) {}

    void AddNotification(const std::string& message, int durationMs) {
        auto now = std::chrono::steady_clock::now();
        auto endTime = now + std::chrono::milliseconds(durationMs);
        notifications.push_back({ notificationIdCounter++, message, now, endTime });
    }

    void DrawNotifications() {
        auto now = std::chrono::steady_clock::now();

        std::sort(notifications.begin(), notifications.end(),
            [now](const Notification& a, const Notification& b) -> bool {
                float durationA = std::chrono::duration_cast<std::chrono::milliseconds>(a.endTime - a.startTime).count();
                float elapsedA = std::chrono::duration_cast<std::chrono::milliseconds>(now - a.startTime).count();
                float percentageA = (durationA - elapsedA) / durationA;

                float durationB = std::chrono::duration_cast<std::chrono::milliseconds>(b.endTime - b.startTime).count();
                float elapsedB = std::chrono::duration_cast<std::chrono::milliseconds>(now - b.startTime).count();
                float percentageB = (durationB - elapsedB) / durationB;

                return percentageA < percentageB;
            }
        );

        int currentNotificationPosition = 0;

        for (auto& notification : notifications) {
            if (now < notification.endTime) {
                float duration = std::chrono::duration_cast<std::chrono::milliseconds>(notification.endTime - notification.startTime).count();
                float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - notification.startTime).count();
                float percentage = (duration - elapsed) / duration * 100.0f;

                ShowNotification(currentNotificationPosition, notification.message, percentage);
                currentNotificationPosition++;
            }
        }

        notifications.erase(std::remove_if(notifications.begin(), notifications.end(),
            [now](const Notification& notification) { return now >= notification.endTime; }),
            notifications.end());
    }

private:
    std::vector<Notification> notifications;
    int notificationIdCounter;

    void ShowNotification(int position, const std::string& message, float percentage) {

        float duePercentage = 100.0f - percentage;
        float alpha = percentage > 10.0f ? 1.0f : percentage / 10.0f;
        GetStyle().WindowPadding = ImVec2(15, 10);

        ImGui::SetNextWindowPos(ImVec2(duePercentage < 15.f ? duePercentage : 15.f, 15 + position * 90));

        ImGui::Begin(("##558" + std::to_string(position)).c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImVec2 pos = ImGui::GetWindowPos(), spacing = ImGui::GetStyle().ItemSpacing, region = ImGui::GetContentRegionMax();
        GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32(c::background::filling, 0.4f), c::elements::rounding);
        GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32(c::background::stroke, alpha), c::elements::rounding);

        CricleProgress("##778", percentage, 100, 7.f, ImVec2(GetContentRegionMax().x - 40, 11));
        ImGui::TextColored(ImColor(GetColorU32(c::accent, alpha)), "%s", "[Notification]");
        ImGui::TextColored(ImColor(GetColorU32(c::elements::text_active, alpha)), "%s", message.c_str());
        ImGui::Dummy(ImVec2(CalcTextSize(message.c_str()).x + 15, 5));

        ImGui::End();
    }
};

NotificationSystem notificationSystem;

KeyAuth keyAuth(name, ownerid, version, url, sellerkey);
std::vector<channel_struct> ChatMessages;
void Messages()
{
    while (true)
    {
        if (iLogin)
        {
            ChatMessages = keyAuth.chatget(XorStr("Test")); //chat name
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void GetNewMessages()
{
    std::string lastMessage;
    while (true)
    {
       if (iLogin)
        {

            if (!ChatMessages.empty())
            {
                if (lastMessage != ChatMessages.back().author)
                {
                    lastMessage = ChatMessages.back().author;
					std::string msgg = "New Message [" + ChatMessages.back().message + "] : \n" + ChatMessages.back().author;
                    notificationSystem.AddNotification(msgg.c_str(), 3000);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}
void SendMessages()
{
    while (true)
    {
        if (iLogin && selectedTab == 1)
        {
        
            keyAuth.chatsend(XorStr("Test").c_str(), input); //chat name
		    input.clear();
            std::this_thread::sleep_for(std::chrono::seconds(1));
			return;
			
        }
    }
}

void RegistKey()
{
	
		if (isRegister) 
		{
	authtextcolor = { 1.00f, 1.00f, 1.00f, 1.00f };
	registInfo = ("Please wait...");
	
	if (!iskeyauthinited1)
	{
		keyAuth.init();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		authtextcolor = { 1.0f, 1.0f, 0.0f, 1.0f };
		registInfo = ("Connected to server...");
		iskeyauthinited1 = true;
	}
	int returnsvr = keyAuth.licenseCheck(username);
	if (returnsvr == 1)
	{
		keyAuth.regist(username);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		authtextcolor = { 0.07f,0.52f,0.17f,1.0f };
		registInfo = ("Account successfully Created...");
		notificationSystem.AddNotification("Account created successfully !", 4000);

	    return;
	}
	else if (returnsvr == 0)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		registInfo = ("Username already taken, try another.");
		notificationSystem.AddNotification("Username already taken !", 4000);
	}
  }
}

bool showCircle = false;
void CheckKey()
{
	if (!isrequestcompleted)
	{
		return;
	}
	showCircle = true;
	isrequestcompleted = false;
	authtextcolor = { 1.00f, 1.00f, 1.00f, 1.00f };
	authtext = ("Preparing for authentication...");
	
	if (!iskeyauthinited)
	{
		keyAuth.init();
		iskeyauthinited = true;
	}
	int returnsvr = keyAuth.license(username);
	if (returnsvr == 0)
	{
		
		authtextcolor = { 0.07f,0.52f,0.17f,1.0f };
		authtext = ("Activated successfully starting...");
		notificationSystem.AddNotification("Welcome to Osiris Hax", 4000);
        std::this_thread::sleep_for(std::chrono::seconds(1));
		showCircle = false;
	     iLogin = true;
		return;
	}
	else if (returnsvr == 1)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("This license has been banned.");
	}
	else if (returnsvr == 10)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Connection failed. Unable to connect to host.");
		isrequestcompleted = true;
		return;
	}
	else if (returnsvr == 2)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Device ID is not match.");
	}
	else if (returnsvr == 3)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Invalid key.");
	}
	else if (returnsvr == 4)
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("License is expired.");
	}
	else
	{
		authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
		authtext = ("Invalid key.");
	}

	isrequestcompleted = true;
	return;
}
char buffer[256];
const char* text = "Welcome to blabla, please login first...";
int length = strlen(text);
float timer = 0.0f;
int index = 0;


void DrawMenu()
{

    if (iLogin)
       {
            ImGui::SetNextWindowPos(ImVec2(glWidth / 2 * 0.5, glHeight / 2 * 0.5 - 150), ImGuiCond_Once);
            static auto Flags1 = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove;
            ImGui::SetNextWindowSize({ 799, 605 });
            if (ImGui::Begin(("@Cheats_Code - Chat Example"), &opened, Flags1))
                
                {
                
                        ImGui::Dummy(ImVec2(0.0f, 25.0f));
                        ImGui::Text(ICON_FA_USER " Users : %s (Online : %s)", myApi.app_data.numUsers.c_str(), myApi.app_data.numOnlineUsers.c_str());
                        ImGui::Spacing();
                        if (!showChat){
                        auto p = ImGui::GetWindowPos(); // Top-left corner of the window
                        auto size = ImGui::GetWindowSize(); // Size of the window
                        auto front = ImGui::GetForegroundDrawList();
                        ImVec2 center = ImVec2(p.x + size.x * 0.5f, p.y + size.y * 0.5f);
                        static ImColor fore_color(255, 255, 0, 215);
                        static ImColor back_color(54, 53, 58);
                        static float arc_size = 0.45f;
                        static float radius = 35.f;
                        static float thickness = 4.f;
                        static float position = 0.f;
                        position = ImLerp(position, IM_PI * 2.f, ImGui::GetIO().DeltaTime * 2.3f);
                        front->PathClear();
                        front->PathArcTo(center, radius, 0.f, 2.f * IM_PI, 40.f);
                        front->PathStroke(ImGui::GetColorU32(back_color.Value), 0, thickness);
                        front->PathClear();
                        front->PathArcTo(center, radius, IM_PI * 1.5f + position, IM_PI * (1.5f + arc_size) + position, 40.f);
                        front->PathStroke(ImGui::GetColorU32(fore_color.Value), 0, thickness);
                        if (position >= IM_PI * 1.90f)
                            showChat = true;
                        ImVec2 text_pos = ImVec2(center.x - ImGui::CalcTextSize("Loading...").x * 0.5f, center.y + radius + 10.0f);
                        front->AddText(text_pos, ImGui::GetColorU32(fore_color.Value), "Loading...");
                        }
                        if (showChat){
                        ImGui::BeginChild("##1users", ImVec2(793, 470), true, 0);
                        {
                        for (channel_struct message : ChatMessages){
                            std::string msg;
                            ImGui::Separator();
                            ImGui::Spacing();
                            ImGui::Image(User.textureID, ImVec2(50, 50));
                            std::string colon = " : \n";
                            msg = message.message.c_str() + colon + message.author.c_str();
                            ImGui::SameLine();
                            ImGui::Text(msg.c_str());
                            ImGui::SameLine();
                            Date = tm_to_readable_time(timet_to_tm(string_to_timet(message.timestamp.c_str())));
                            ImGui::TextColored(ImVec4(0.587f, 0.587f, 0.587f, 1.0f), Date.c_str());
                         }
                           ImGui::SetScrollHereY(1.0f);
                           ImGui::EndChild();
                    }
                           ImGui::Spacing();
                            
                           ImGui::PushItemWidth(650);
                           ImGui::KeypadEditString(msgErr.c_str(), &input);
                           ImGui::PopupKeypad(ImGui::GetFont());
                           ImGui::SameLine();
                           if (ImGui::Button(XorStr("Send").c_str(), ImVec2(80, 25))) 
                               {
                                   new std::thread(SendMessages);
                                   
                                }
                            }
                    }
          }else{
          
      	ImVec2 window_pos = ImVec2(glWidth / 2, glHeight / 2);
  	    ImVec2 window_pos_pivot = ImVec2(0.5f, 0.5f); // Center pivot
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Once, window_pos_pivot);
		if (ImGui::Begin(" Login Page", 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove))

			ImGui::Text(" "); ImGui::SameLine();
			//ImGui::Image(Logo.textureID, ImVec2(130, 130));
			ImGui::Spacing();
			ImGui::Text(" ");
			ImGui::SameLine();
		    TextURL("Discord", "https://t.me/Cheats_Code", 0, 0);
		    linevertical();
		    TextURL("Telegram", "https://t.me/Cheats_Code", 0, 0);
			ImGui::Spacing();
			ImGui::Text(" ");
			ImGui::SameLine();
			ImGui::Text("Version :");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "v-1.0-beta");
			ImGui::SetCursorPos(ImVec2(160, 25));
			if (!isRegister){
			ImGui::BeginChild("##kmssss", ImVec2(415, 200), true, 0);
			timer += ImGui::GetIO().DeltaTime;

    if (timer > 0.2f && index < length)
    {
        timer -= 0.2f;
        buffer[index++] = text[index];
        buffer[index] = '\0';
    }
	ImGui::Dummy(ImVec2(1, 0)); ImGui::SameLine();
	ImGui::Text("%s", buffer);
	ImGui::Spacing();
			ImGui::Dummy(ImVec2(1, 10)); 
				if (authtext.size() > 0)
{
    float window_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(authtext.c_str()).x;
    ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
    
    ImGui::PushStyleColor(ImGuiCol_Text, authtextcolor);
    ImGui::Text(authtext.c_str());
    ImGui::PopStyleColor();
}


		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, 255));
		ImGui::Text("   Username:"); ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::SetNextItemWidth(300);
		ImGui::KeypadEditString("Username...", &username);
		ImGui::PopupKeypad(ImGui::GetFont());
		ImGui::Dummy(ImVec2(1, 0)); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		if (ImGui::Button("Login", ImVec2(190, 40)))
		{
			new std::thread(CheckKey);
		}
		
		ImGui::SameLine();
		if (ImGui::Button("  Paste", ImVec2(190, 40))){
			auto key = getClipboardText();
			username.resize(key.size() + 1);
			strncpy(&username[0], key.c_str(), key.size());
			username[key.size()] = '\0';
			}
			
		ImGui::Spacing();
		ImGui::Dummy(ImVec2(1, 0)); ImGui::SameLine();
		if (ImGui::Button("Create an Account", ImVec2(388, 40)))
		{
			isRegister = true;
			username.clear();
		}
		}else{
		ImGui::BeginChild("##kmsssssi", ImVec2(415, 220), true, 0);
			timer += ImGui::GetIO().DeltaTime;

    if (timer > 0.2f && index < length)
    {
        timer -= 0.2f;
        buffer[index++] = text[index];
        buffer[index] = '\0';
    }
	ImGui::Dummy(ImVec2(1, 0)); ImGui::SameLine();
	ImGui::Text("%s", buffer);
	ImGui::Spacing();
			ImGui::Dummy(ImVec2(1, 10)); 
				if (registInfo.size() > 0)
{
    float window_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(registInfo.c_str()).x;
    ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
    
    ImGui::PushStyleColor(ImGuiCol_Text, authtextcolor);
    ImGui::Text(registInfo.c_str());
    ImGui::PopStyleColor();
}
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, 255));
		ImGui::Text("   Username:"); ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::SetNextItemWidth(300);
		ImGui::KeypadEditString("Username...", &username);
		
		ImGui::Spacing();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 255, 255, 255));
		ImGui::Text("   Password: "); ImGui::SameLine();
		ImGui::PopStyleColor();
		ImGui::SetNextItemWidth(300);
		ImGui::KeypadEditString("Password...", &password);
		ImGui::PopupKeypad(ImGui::GetFont());
		ImGui::Dummy(ImVec2(1, 0)); ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
		if (ImGui::Button("Sign Up", ImVec2(388, 40)))
		{
			if (username.size() > 0 && password.size() > 0)
			     {
			
			new std::thread(RegistKey);
			}
		    if (username.size() == 0 && password.size() == 0){
			authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
			registInfo = ("Username and password cannot be empty !");
			}else if (username.size() == 0){
			authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
			registInfo = ("Username cannot be empty !");
			}else if (password.size() == 0){
			authtextcolor = { 1.0f,0.0f,0.0f,1.0f };
			registInfo = ("Password cannot be empty !");
			}
		}
		ImGui::Spacing();
		ImGui::Dummy(ImVec2(1, 0)); ImGui::SameLine();
		if (ImGui::Button("Back to login", ImVec2(388, 40)))
		{
			isRegister = false;
		}
	}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::EndChild();
		ImGui::End();
    }
}

// ======================================================================== //
#define IM_CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
namespace Settings
{
    static int Tab = 0;
	static int SkinTab = 0;
}
EGLBoolean (*orig_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);

EGLBoolean _eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    if (glWidth <= 0 || glHeight <= 0)
        return orig_eglSwapBuffers(dpy, surface);

    if (!g_App)
        return orig_eglSwapBuffers(dpy, surface);

    screenWidth = ANativeWindow_getWidth(g_App->window);
    screenHeight = ANativeWindow_getHeight(g_App->window);
    density = AConfiguration_getDensity(g_App->config);
    if (!initImGui) {
        ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGuiStyle *style = &ImGui::GetStyle();
         ImGui::StyleColorsDark(style);
        style->ItemSpacing = ImVec2(8, 5);
        style->ItemInnerSpacing = ImVec2(5, 8);
        style->CellPadding = ImVec2(5, 5);
        style->IndentSpacing = 27.0f;
        style->ScrollbarSize = 18.0f;
        style->ScrollbarRounding = 9.0f;
        style->GrabMinSize = 5.0f;
        style->GrabRounding = 3.0f;
        style->PopupRounding = 2.f;
        style->Alpha = 0.95;
        style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style->TouchExtraPadding = ImVec2(0, 0);
        style->TabRounding = 7.0f;
        style->ChildRounding = 7.0f;
        style->ButtonTextAlign = ImVec2(0.50f, 0.50f);
        style->ChildBorderSize = 0.100f;
        style->DisplayWindowPadding = ImVec2(22, 22);
        style->DisplaySafeAreaPadding = ImVec2(4, 4);
        style->AntiAliasedLines = true;
        style->CurveTessellationTol = 1.f;
        style->TabBorderSize = 0.000f;
        ImGui_ImplAndroid_Init();
        ImGui_ImplOpenGL3_Init("#version 300 es");
		static const ImWchar icons_ranges[] = { ICON_MIN_FA	, ICON_MAX_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.OversampleH = 2.5;
		icons_config.OversampleV = 2.5;
		ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 18.f, &font_cfg);
	io.Fonts->AddFontFromMemoryTTF((void*)fontAwesome, sizeof(fontAwesome), 17, &icons_config, icons_ranges);
	User = CreateTexture(user, sizeof(user));// LoadTextureFromURL("https://xhunterr.com/apk/user.png");
   
        initImGui = true;	
    }

        ImGuiIO &io = ImGui::GetIO();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
        ImGui::NewFrame();
        CustomImGUIStyle();
		notificationSystem.DrawNotifications();
		DrawMenu();
	    ImGui::End();
	    ImGui::Render();
    	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	    return orig_eglSwapBuffers(dpy, surface);
}

void (*orig_onInputEvent)(void *inputEvent, void *ex_ab, void *ex_ac);
void onInputEvent(void *inputEvent, void *ex_ab, void *ex_ac) {
orig_onInputEvent(inputEvent, ex_ab, ex_ac);if (initImGui) {ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)inputEvent, {(float) screenWidth / (float) glWidth, (float) screenHeight / (float) glHeight});}}


[[noreturn]] void *maps_thread(void *)
{
	while (true)
	{
		
		auto t1 = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		std::vector<sRegion> tmp;
		char line[512];
		FILE *f = fopen("/proc/self/maps", "r");
		if (f)
		{
			while (fgets(line, sizeof line, f))
			{
				uintptr_t start, end;
				char tmpProt[16];
				if (sscanf(line, "%" PRIXPTR "-%" PRIXPTR " %16s %*s %*s %*s %*s", &start, &end, tmpProt) > 0)
				{
					if (tmpProt[0] != 'r')
					{
						tmp.push_back({start, end});
					}
				}
			}
			fclose(f);
		}
		trapRegions = tmp;
		auto td = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - t1;
		std::this_thread::sleep_for(std::chrono::milliseconds(std::max(std::min(0LL, SLEEP_TIME - td), SLEEP_TIME)));
	}
}

#include <dlfcn.h>
#include "fake_dlfcn.h"


void *main_thread(void *)
{
	LOGI("MainThread Started");
	UE4 = Tools::GetBaseAddress("libUE4.so");
	while (!UE4)
	{
		UE4 = Tools::GetBaseAddress("libUE4.so");
		sleep(1);
	}

	while (!g_App)
	{
		g_App = *(android_app **)(UE4 + GNativeAndroidApp_Offset);
		sleep(1);
	}

	void *input = dlopen_ex(OBFUSCATE("libinput.so"), 4);
    while (!input) {input = dlopen_ex(OBFUSCATE("libinput.so"), 4);
    sleep(1);}void *address = dlsym_ex(input, OBFUSCATE("_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE"));HOOK(address, onInputEvent, &orig_onInputEvent);dlclose_ex(input);
    void *egl = dlopen_ex("libEGL.so", 4);
    while (!egl) {
    egl = dlopen_ex("libEGL.so", 4);
    sleep(1);}
	DobbyHook((void *) dlsym_ex(egl, "eglSwapBuffers"), (void *) _eglSwapBuffers, (void **) &orig_eglSwapBuffers);
	pthread_t t;
	pthread_create(&t, nullptr, maps_thread, nullptr);
	new std::thread(Messages);
	new std::thread(GetNewMessages);
	return nullptr;
}

__attribute__((constructor)) void _init()
{
	pthread_t t;
	pthread_create(&t, 0, main_thread, 0);
}
