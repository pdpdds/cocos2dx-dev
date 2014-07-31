function start()
	scene.desc("You are deep undercover with the mafia.");
	scene.anim("Open door");
	scene.anim("Enter officer");
	scene.anim("Louie looks away");
	scene.dialog("Officer: Alright Big Louie. This is a raid. You're under arrest for the murder of Frankie Boy Caruso.");
	scene.anim("Pull guns");
	scene.dialog("Big Louie: Murder? What's a murder?");
	scene.dialog("Officer: Don't play dumb with me.");
	scene.dialog("Big Louie: A one-man raid? You must have a death wish.");
	scene.anim("Louie looks at you");
	scene.dialog("Big Louie: What do YOU think we should do with him?");
	
	scene.dialogOption("You can't take him out now. There are too many witnesses.");
	scene.dialogOption("I say take him out. He's here alone.");
	scene.dialogOption("He's a cop. We'll have a mess on our hands if we take him down.");

	scene.presentOptions();
	
	if scene.getResponse() == 1 then
		tooManyWitnesses();
	elseif scene.getResponse() == 2 then
		hereAlone();
	elseif scene.getResponse() == 3 then
		bigMess();
	end
end

function tooManyWitnesses()
	scene.dialog("Big Louie: Whaddya mean too many witnesses? These are all my men...");
	scene.anim("Louie scowls");
	scene.dialog("Big Louie: ...and YOU! Blast him boys.");
	script.waitSeconds(1);
	scene.anim("Gun pointed at you");
	scene.desc("You are dead.");
end


function bigMess()
	scene.dialog("Big Louie: I don't like it, but, you're right.");
	scene.dialog("Big Louie: Men, you can put your guns down.");
	scene.logic("Put guns down");
	scene.anim("Put guns down");
	scene.desc("Louie's men lower their weapons");
	scene.anim("Louie looks away");
	scene.dialog("Big Louie: Cop, looks like you have a new lease on life.");
	
	scene.actionOption("Pull your gun on Big Louie");
	scene.actionOption("Pull your gun on Big Louie's men");
	
	scene.presentOptions();
	
	if scene.getResponse() == 1 then
		pullGunOnLouie();
	elseif scene.getResponse() == 2 then
		pullGunOnMen();
	end
end


function hereAlone()
	scene.anim("Officer shocked");
	script.waitSeconds(1);
	scene.dialog("Officer: Jerry! What the hell are you doing?!");
	scene.anim("Louie scowls");
	scene.dialog("Big Louie: Jerry? You lying scumbag! We trusted you...blast him boys.");
	script.waitSeconds(1);
	scene.anim("Gun pointed at you");
	scene.desc("You are dead.");
end


function pullGunOnLouie()
	scene.anim("Pull gun on Louie");
	scene.anim("Louie scowl");
	scene.dialog("Big Louie: This guy's a Fed! Blast him!");
	script.waitSeconds(1);
	scene.anim("Gun pointed at you");
	scene.desc("You are dead.");	
end


function pullGunOnMen()
	gunsDown = scene.logic("Are guns down?");
	scene.anim("Pull gun on men");
	if gunsDown == 1 then
		scene.anim("Louie scowls");
		scene.dialog("Big Louie: You played me for a fool!");
		scene.anim("Louie looks away");
		scene.dialog("Officer: You're under arrest Big Louie.");
		scene.dialog("Big Louie: I'll be back on the streets in twenty-four hours!");
		scene.dialog("Officer: We'll try to make it twelve.");
		scene.anim("Louie looks at you");
		scene.logic("You win");
		scene.desc("You win!");
	else
		scene.anim("Louie scowls");
		scene.dialog("Big Louie: This guy's a Fed! Blast him!");
		script.waitSeconds(1);
		scene.anim("Gun pointed at you");
		scene.desc("You are dead.");		
	end
end

start();