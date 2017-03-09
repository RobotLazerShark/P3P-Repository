require "C++Binder"


--start is called only once, at startup. Reloading the file will not call start again.
function start ()
	explainedControls = false;
end

--main is the default function the program keeps updating.
function main ()
	if Level.isHub ()
	then
		if Player.completedQuests () == 0 and not explainedControls
		then
			explainControls ()
		elseif Npc.isTalking ()--You have to walk up to the Npc to talk to him
		then
			if Player.questTalks () > 0
			then
				getAnnoyed (Player.questTalks ())
			else
				explainQuest (Player.completedQuests (), true)
			end
		end
	end
end


--explain to the player how the controls work, and give the first questTalks
function explainControls ()
	text = "Memory reset completed."
	pause = 0.3
	Player.lock (true);--Prevent the player from moving about while we're talking
	Lua.wait (1.5)--wait for everything to get set up, and for the player to get used to what (s)he's seeing.
	Npc.showDialog (text, false)
	Lua.wait (pause)
	text = text.."\n\tNo backups found."
	Npc.updateDialog (text)
	Lua.wait (pause + 0.65)
	text = "REBOOTING"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nRestarting controller module."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n\tLoading commands:"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Left, A/LeftArrow"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Right, D/RightArrow"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Forward, W/UpArrow"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Backward, S/DownArrow"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n\tRebuild, Time = Reversed, R"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nController module restarted."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nRestarting all other systems"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nREBOOT DONE."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n<space = continue>"
	Npc.updateDialog (text)
	Player.waitForKey ()--This will pause the lua program until SPACE has been pressed.
	text = "G|¡t©h:"
	Npc.showDialog (text, true)
	Lua.wait (pause)
	text = text.."\nWäkE up! I neED y0uR hE|p!"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n|'m verY unsTABle r¡ght ñ0w, as yOU ©àn see."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nI nEEd yoü To gET a pUñctúàt¡0n CHïp frõm roóm 1B,"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nìt sHoú1d Fï× My ©oms."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nI'll opEn thE dóor for y0u, iT's 0n thÈ r¡gHt."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\n|f you sEÈ à c0MpuTéR, wa|k úP to ït ANd 1'll çõnta©T you"
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nw¡tH fùrhTËr íñstRüçti0ns. Yoú'l1 nëEd tHe he|p."
	Npc.updateDialog (text)
	Player.giveQuest ("QuestItem1", 0.4)
	Level.setKey (3)--unlock level 1, 2 and 3 (1A, 1B, 1C)
	Player.waitForKey ()
	Npc.stopDialog ()--Hide the textbox and text
	Player.lock (false)--The player is now allowed to move again
	explainedControls = true--We won't explain the controls again
end

--the player just completed a quest, give and explain the new quest.
function explainQuest (pCurrentQuest, pGiveNewQuest)
	if pCurrentQuest == 0--Quest 1 is explained and given when the controls are explained
	then
		Player.lock (true)
		text = "G|¡t©hèd B0t:"
		Npc.showDialog (text)
		Lua.wait (pause)
		text = text.."\nP1éãSe, g0 gët thE pARt | nEÈd!"
		Npc.updateDialog (text)
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	elseif pCurrentQuest == 1
	then
		Player.lock (true)
		text = "Glitched Bot:"
		Npc.showDialog (text)
		Lua.wait (pause)
		text = text.."\nThanks, my coms are fully functional now!"
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nIf you can find all the parts to fix me completely,"
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nI can retrieve your memory backup from the core server."
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nBut first, I need you to find more parts."
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\n"
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nI opened the door for you, its on the left this time."
		Npc.updateDialog (text)
		if pGiveNewQuest
		then
			Player.giveQuest ("QuestItem2", 0.3)
			Level.setKey (6)--unlock 2A, 2B, 2C
		end
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	elseif pCurrentQuest == 2
	then
		Player.lock (true)
		text = "Glitched Bot:"
		Npc.showDialog (text)
		Lua.wait (pause)
		text = text.."\nAh, much better!"
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nThere is only one more thing that needs fixing."
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nMy... welding units. They are very important to me."
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nThe door is on the right."
		Npc.updateDialog (text)
		if pGiveNewQuest
		then
			Player.giveQuest ("QuestItem3", 0.3)
			Level.setKey (9)--unlock 3A, 3B, and bosslevel
		end
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	else
		--The last quest has been completed
		Player.lock (true)
		text = "Evil Bot:"
		Npc.showDialog (text)
		Lua.wait (pause * 2)
		text = text.."\nFinally, I am whole again!"
		Npc.updateDialog (text)
		Lua.wait (pause * 2)
		text = text.."\nNow I can continue my plans of world domination!"
		Npc.updateDialog (text)
		Lua.wait (pause * 4)
		text = text.."\nThe only one left in my way is you..."
		Npc.updateDialog (text)
		Lua.wait (pause * 6)
		Sound ("EvilLaugh.wav")
		text = text.."\nHA HA HA HA HA HA HA HA"
		Npc.updateDialog (text)
		Lua.wait (pause * 2)
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
		Level.loadBossLevel ()
	end
end

--the player wants to talk again about a quest we already explained
function getAnnoyed (pTalksHad)
	if pTalksHad < 3--The first time we explain the quest is before we've actually given it to the player. So talks had will be 1 less than the amount of times we've explained the quest.
	then
		--explain the quest again
		explainQuest (Player.completedQuests () + 1, false)
	else
		Player.lock (true)
		text = "Glitched Bot:"
		Npc.showDialog (text)
		Lua.wait (pause)
		text = text.."\nCome on, I already told you what to do!"
		Npc.updateDialog (text)
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	end
end