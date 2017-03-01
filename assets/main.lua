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
	local text = "REBOOTING"
	local pause = 0.3
	Player.lock (true);--Prevent the player from moving about while we're talking
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\nRestarting controller module."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\n\tLoading commands:"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Left, A/LeftArrow"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Right, D/RightArrow"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Forward, W/UpArrow"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\n\tMove, Direction = Backward, S/DownArrow"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\nController module restarted."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\nRestarting all other systems"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."."
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\nREBOOT DONE.\n"
	Npc.showDialog (text)
	Lua.wait (pause)
	text = text.."\n<space = continue>"
	Npc.showDialog (text)
	Player.waitForKey ()--This will pause the lua program until SPACE has been pressed.
	Npc.updateDialog ("G|¡t©h:\nWäkE up! I neED y0uR hE|p!\n|'m verY unsTABle r¡ght ñ0w, as yOU ©àn see.\nI nEEd yoü To gET the yel|ow blõCk ¡ñ roóm A2.\nìt sHoú1d FïX My ©oms.\nI'll opEn thE dóor for y0u.")
	Player.giveQuest ("Item 1", 0.5)
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
		Npc.showDialog ("G|¡t©h:\nP1éãSe, g0 gët thE pARt!")
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	elseif pCurrentQuest == 2
	then
		Player.lock (true)
		Npc.showDialog ("G|¡t©h:\nThanks, my coms are fully functional now!\nNow I need you to find the next part...\nI opened the door for you, its on the right.")
		Player.waitForKey ()
		if pGiveNewQuest
		then
			Player.giveQuest ("Item 2", 0.5)
		end
		Npc.stopDialog ()
		Player.lock (false)
	elseif pCurrentQuest > 2
	then
		--The last quest has been completed
		Player.lock (true)
		Npc.showDialog ("Finally, I am whole again!\n*Ominous laughter*")
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
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
		Npc.showDialog ("Come on, I already told you what to do 3 times!")
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	end
end