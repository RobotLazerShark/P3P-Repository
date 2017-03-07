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
				explainQuest (Player.completedQuests () + 1, true)
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
	text = text.."\nI nEEd yoü To gET a pUñctúàt¡0n CHïp frõm roóm A2."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nìt sHoú1d Fï× My ©oms."
	Npc.updateDialog (text)
	Lua.wait (pause)
	text = text.."\nI'll opEn thE dóor for y0u."
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
	Player.giveQuest ("QuestItem1", 0.5)
	Level.setKey (2);--unlock level 1, room 1 and 2
	Player.waitForKey ()
	Npc.stopDialog ()--Hide the textbox and text
	Player.lock (false);--The player is now allowed to move again
	explainedControls = true--We won't explain the controls again
end

--the player just completed a quest, give and explain the new quest.
function explainQuest (pCurrentQuest, pGiveNewQuest)
	if pCurrentQuest == 1--Quest 1 is explained and given when the controls are explained
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
	elseif pCurrentQuest == 2
	then
		Player.lock (true)
		text = "Glitched Bot:"
		Npc.showDialog (text)
		Lua.wait (pause)
		text = text.."\nThanks, my coms are fully functional now!"
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nNow I need you to find the next part, so I can patch my..."
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\ncleanup protocols."
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\n"
		Npc.updateDialog (text)
		Lua.wait (pause)
		text = text.."\nI opened the door for you, its on the left.\n[NOTE: level not actually build yet!]"
		Npc.updateDialog (text)
		Player.waitForKey ()
		if pGiveNewQuest
		then
			Player.giveQuest ("Item 2", 0.5)
			Level.setKey (4)--unlock room 4
		end
		Npc.stopDialog ()
		Player.lock (false)
	elseif pCurrentQuest > 2
	then
		--The last quest has been completed
		Player.lock (true)
		text = "Evil Bot:"
		Npc.showDialog (text)
		Lua.wait (pause)
		Sound ("EvilLaugh.wav")
		text = text.."Finally, I am whole again!\n"
		Npc.updateDialog (text)
		Lua.wait (1.5)
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
		loadBossLevel ()
	end
end

--the player wants to talk again about a quest we already explained
function getAnnoyed (pTalksHad)
	if pTalksHad < 3--The first time we explain the quest is before we've actually given it to the player. So talkshad will be 1 less than the amount of times we've explained the quest.
	then
		--explain the quest again
		explainQuest (Player.completedQuests () + 1, false)
	else
		Player.lock (true)
		text = "Glitched Bot:"
		Npc.showDialog (text)
		Lua.wait (pause)
		Npc.showDialog ("Come on, I already told you what to do 3 times!")
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	end
end