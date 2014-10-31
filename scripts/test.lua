


for i=0,4 do
	mBox(i*64, 400)
end



pos = createVec2(100,100);
p = player(pos);

function p()
	player(getMousePos());
end

function createBoxes()
	for i=0,12 do
		mBox(i*64, 200)
	end
end
