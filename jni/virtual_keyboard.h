#pragma once

static std::vector<unsigned int> keys_pressed;
static int refocus=0;

namespace ImGui
{
    static std::map<const char *, int> g_KeypadApplyMap; // map of labels to return values
    static char * g_KeypadCurrentLabel; // only one instance of Keypad is open at any one time
    static std::string  *g_KeypadEditStrPtr;  // pointer to string to edit
    static std::string  g_KeypadEditStrRestore; // stored value for undo
    
    // Draw a 4x5 button matrix entry keypad edits a *value std::string,
    // scaled to the current content region height with square buttons    
    static int InputKeypad(ImFont *font, const char *label, bool* p_visible, std::string *value) {
        int ret = 0; 

        if (p_visible && !*p_visible)
            return ret;  
        if (!value || !label)
            return ret;

        ImVec2 csize = GetContentRegionAvail();
        int n = (csize.y / 5); // height / 5 button rows
      
        ImGuiStyle &style = GetStyle();
      
        if (BeginChild(label, ImVec2(0,0), true)) {
            
            csize = GetContentRegionAvail(); // now inside this child
            n = (csize.y / 5) - style.ItemSpacing.y; // button size
            ImVec2 bsize(n + 63, n - 5); // buttons are square
			ImVec2 nsize(n + 50, n - 12);
            PushStyleVar(ImGuiStyleVar_FrameRounding, 6);
			static std::string k = "";
			static bool bigText = false;
			static bool other = false;
            PushFont(font);
			ImGui::SetCursorPos(ImVec2(300, 1));
			
			 InputTextWithHint(std::string(std::string("##") + std::string(label)).c_str(), "Message...", value->data(), value->capacity());
			if (Button("ESC", {130, n -12}))
			{
				k = "ESC";
			}
			//Number
			SameLine();
			if (Button("1", nsize))
			{
				k = "1";
			}
			SameLine();
			if (Button("2", nsize))
			{
				k = "2";
			}
			SameLine();
			if (Button("3", nsize))
			{
				k = "3";
			}
			SameLine();
			if (Button("4", nsize))
			{
				k = "4";
			}
			SameLine();
			if (Button("5", nsize))
			{
				k = "5";
			}
			SameLine();
			if (Button("6", nsize))
			{
				k = "6";
			}
			SameLine();
			if (Button("7", nsize))
			{
				k = "7";
			}
			SameLine();
			if (Button("8", nsize))
			{
				k = "8";
			}
			SameLine();
			if (Button("9", nsize))
			{
				k = "9";
			}
			SameLine();
			if (Button("0", nsize))
			{
				k = "0";
			}
            
			ImGui::SetCursorPosX(35);
			//Unicode 1
			if (Button(bigText ? "Q" : other ? "@" : "q", bsize))
			{
				k = bigText ? "Q" : other ? "@" : "q";
			}
			SameLine();
			if (Button(bigText ? "W" : other ? "#" : "w", bsize))
			{
				k = bigText ? "W" : other ? "#" : "w";
			}
			SameLine();
			if (Button(bigText ? "E" : other ? "$" : "e", bsize))
			{
				k = bigText ? "E" : other ? "$" : "e";
			}
			SameLine();
			if (Button(bigText ? "R" : other ? "_" : "r", bsize))
			{
				k = bigText ? "R" : other ? "_" : "r";
			}
			SameLine();
			if (Button(bigText ? "T" : other ? "&" : "t", bsize))
			{
				k = bigText ? "T" : other ? "&" : "t";
			}
			SameLine();
			if (Button(bigText ? "Y" : other ? "-" : "y", bsize))
			{
				k = bigText ? "Y" : other ? "-" : "y";
			}
			SameLine();
			if (Button(bigText ? "U" : other ? "+" : "u", bsize))
			{
				k = bigText ? "U" : other ? "+" : "u";
			}
			SameLine();
			if (Button(bigText ? "I" : other ? "(" : "i", bsize))
			{
				k = bigText ? "I" : other ? "(" : "i";
			}
			SameLine();
			if (Button(bigText ? "O" : other ? ")" : "o", bsize))
			{
				k = bigText ? "O" : other ? ")" : "o";
			}
			SameLine();
			if (Button(bigText ? "P" : other ? "/" : "p", bsize))
			{
				k = bigText ? "P" : other ? "/" : "p";
			}
			
			ImGui::SetCursorPosX(35);
			//Unicode 2
			if (Button(bigText ? "A" : other ? "*" : "a", bsize))
			{
				k = bigText ? "A" : other ? "*" : "a";
			}
			SameLine();
			if (Button(bigText ? "S" : other ? "\"" : "s", bsize))
			{
				k = bigText ? "S" : other ? "\"" : "s";
			}
			SameLine();
			if (Button(bigText ? "D" : other ? "'" : "d", bsize))
			{
				k = bigText ? "D" : other ? "'" : "d";
			}
			SameLine();
			if (Button(bigText ? "F" : other ? ":" : "f", bsize))
			{
				k = bigText ? "F" : other ? ":" : "f";
			}
			SameLine();
			if (Button(bigText ? "G" : other ? ";" : "g", bsize))
			{
				k = bigText ? "G" : other ? ";" : "g";
			}
			SameLine();
			if (Button(bigText ? "H" : other ? "!" : "h", bsize))
			{
				k = bigText ? "H" : other ? "!" : "h";
			}
			SameLine();
			if (Button(bigText ? "J" : other ? "?" : "j", bsize))
			{
				k = bigText ? "J" : other ? "?" : "j";
			}
			SameLine();
			if (Button(bigText ? "K" : other ? "~" : "k", bsize))
			{
				k = bigText ? "K" : other ? "~" : "k";
			}
			SameLine();
			if (Button(bigText ? "L" : other ? "`" : "l", bsize))
			{
				k = bigText ? "L" : other ? "`" : "l";
			}
			SameLine();
			if (Button(ICON_FA_ERASER, bsize))
			{
				k = "DEL";
			}
            
			ImGui::SetCursorPosX(45);
			//Unicode 2
			static int clicked = 0;
        
			if (Button(ICON_FA_ANGLE_DOUBLE_UP, {150, n -4}))
				clicked++;
				bigText = false;
			if (clicked & 1)
              {
				//k = "UP";
				bigText = true;
			}
			SameLine();
			if (Button(bigText ? "Z" : other ? "|" : "z", bsize))
			{
				k = bigText ? "Z" : other ? "|" : "z";
			}
			SameLine();
			if (Button(bigText ? "X" : other ? "{}" : "x", bsize))
			{
				k = bigText ? "X" : other ? "{}" : "x";
			}
			SameLine();
			if (Button(bigText ? "C" : other ? "\/" : "c", bsize))
			{
				k = bigText ? "C" : other ? "\/" : "c";
			}
			SameLine();
			if (Button(bigText ? "V" : other ? "%" : "v", bsize))
			{
				k = bigText ? "V" : other ? "%" : "v";
			}
			SameLine();
			if (Button(bigText ? "B" : other ? "%" : "b", bsize))
			{
				k = bigText ? "B" : other ? "%" : "b";
			}
			SameLine();
			if (Button(bigText ? "N" : other ? "[" : "n", bsize))
			{
				k = bigText ? "N" : other ? "[" : "n";
			}
			SameLine();
			if (Button(bigText ? "M" : other ? "]" : "m", bsize))
			{
				k = bigText ? "M" : other ? "]" : "m";
			}
			SameLine();
			if (Button("Enter", {180, n - 4}))
			{
				k = "ENTER";
			}
            
			ImGui::SetCursorPosX(85);
			//Unicode 3
			if (Button("?#@", {180, n -4}))
			{
				k = "OTHER";
			}
			SameLine();
			if (Button(",", bsize))
			{
				k = ",";
			}
			SameLine();
			if (Button("Space", {600, n -4}))
			{
				k = " ";
			}
			SameLine();
			if (Button(".", bsize))
			{
				k = ".";
			}
			
			SameLine();
			if (Button("Clear", {180, n -4}))
			{
				k = "CLEAR";
			}
            PopFont();
			PopStyleVar();
			
            // logic
            if (k != "") {
                if (k != "ENTER" && k != "ESC" && k != "DEL" && k != "CLEAR" && k != "OTHER" && k != "UP"){
                    value->append(k); // add k to the string
                } 
                else {
                    if (k == "ENTER") { // enter                  
                        ret = 1; // value has been accepted        
						
                    }
                    else if (k == "DEL") { // remove one char from the string
                        std::string tvalue = value->substr(0, value->length() - 1);
                        value->swap(tvalue);
                    } else if (k == "CLEAR"){
                        value->clear();
                    }
                    else if (k == "ESC") { // cancel                        
                        ret = -1; //  restore old value
                    }
                    
					if (k == "UP")
					{                        
                        bigText = true; //  change to upper character
                    }
					else
					{                        
                        bigText = false; //  change to upper character
                    }
					if (k == "OTHER")
					{                        
                        other = true; //  change to upper character
                    }
					else
					{                        
                        other = false; //  change to upper character
                    }
					
                }
                if (ret) *p_visible = false;
            }            
            k = "";        
            EndChild();           
        }    
        g_KeypadApplyMap[label] = ret; // store results in map
        return ret;
    }

    // The widget takes a label, and a *std::string
    // returns 1 if the new value should accepted, enter pressed
    // returns 0 if nothing happened
    // return -1 if cancel pressed, previous value has been restored     
    static int KeypadEditString(const char *label, std::string *value){
        if (!label || !value) return 0;

        //Text(label);
        //SameLine();
        //InputTextMultiline("##source", value->data(), value->capacity(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16));
        InputTextWithHint(std::string(std::string("##") + std::string(label)).c_str(), label, value->data(), value->capacity()); 
    
        if (IsItemHovered() && IsMouseClicked(ImGuiMouseButton_Left)) {        
            if (value != g_KeypadEditStrPtr){
                g_KeypadEditStrRestore = value->c_str();
                g_KeypadEditStrPtr = value;   
                g_KeypadApplyMap[label] = 0;      
                g_KeypadCurrentLabel = (char*)label;   
            }
            OpenPopup(ICON_FA_KEYBOARD" Keyboard"); 
        }

        if (g_KeypadApplyMap[label] == 1) {      
            g_KeypadApplyMap[label] = 0;
            return 1;
        } else if (g_KeypadApplyMap[label] == -1) {
            g_KeypadApplyMap[label] = 0;
            return -1;
        }
    return 0;
    }

    // Show the popup keypad box as required.
    static void PopupKeypad(ImFont *font){
        // Always center this window when appearing
        ImGuiIO& io = ImGui::GetIO();
ImVec2 window_pos = ImVec2(0, io.DisplaySize.y / 2);
ImVec2 window_size = ImVec2(io.DisplaySize.x, 470); // Screen width and desired height

SetNextWindowPos(window_pos, ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
SetNextWindowSize(window_size);
float original_scale = ImGui::GetIO().FontGlobalScale;
    if (BeginPopupModal(ICON_FA_KEYBOARD" Keyboard", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
		ImGui::GetIO().FontGlobalScale = 1.8;
            if (g_KeypadEditStrPtr != nullptr) {
                bool gShowKeypad = true;                       
                //TextWrapped(g_KeypadEditStrPtr->c_str());
                int r = InputKeypad(font, "Keypad Input", &gShowKeypad, g_KeypadEditStrPtr);
                if (r == -1) {
                    // undo - restore previous value
                    g_KeypadEditStrPtr->swap(g_KeypadEditStrRestore);                
                    g_KeypadApplyMap[g_KeypadCurrentLabel] = -1;
                }
                else if (r == 1) {
                    // set - we should apply the new value 
                    g_KeypadApplyMap[g_KeypadCurrentLabel] = 1;               
                }
                if (!gShowKeypad) CloseCurrentPopup();            
            }
            EndPopup();
			ImGui::GetIO().FontGlobalScale = original_scale;
        }
    }

} // Namespace ImGui
