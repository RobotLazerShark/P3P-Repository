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
	Player.lock (true);--Prevent the player from moving about while we're talking
	Npc.showDialog ("Hello!\nUse the arrowkeys or WASD to move around.\nPress R to restart at any time.\nPress SPACE to continue dialog.\n\t<[Press SPACE to continue]>")
	Player.waitForKey ()--This will pause the lua program until SPACE has been pressed.
	Npc.updateDialog ("I will now give you a quest.\nI need you to find an item for me.\nIt is a [visual description].\nIt's at the end of Level 2, but you\nwill need to go through level 1 first.\n\t<[Press SPACE to continue]>")
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
		Npc.showDialog ("I told you already, go get my item!\nStart with level 1!")
		Player.waitForKey ()
		Npc.stopDialog ()
		Player.lock (false)
	elseif pCurrentQuest == 2
	then
		Player.lock (true)
		Npc.showDialog ("Well done! You found Item1!\nNow go find Item 2 for me.")
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
		Npc.showDialog ("Congratulations, you found all the items!\nNow I am fully functional again!\n*Ominous laughter*")
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