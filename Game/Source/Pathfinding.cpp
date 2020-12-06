#include "Pathfinding.h"

bool Pathfinding::IFindPath(iPoint& start, iPoint& end, short maxCharacterJumpHeight, int characterHeight, int characterWidth) {

	while (touchedLocations.size() > 0)
	{
		int deleteIndex = touchedLocations.top();
		touchedLocations.pop();
		nodes[deleteIndex].clear();
	}

	if (costGrid[end.x][ end.y] == 0)
		return false;

	found = false;
	//stop = false;
	//stopped = false
	closedNodeCounter = 0;
	openNodeValue += 2;
	closedNodeValue += 2;
	open.Clear();
	//TODO: a males puc declarar la priority aqui
	//open.clear();
	//close.clear();
	location.xy = start.y * gridWidth + start.x;
	location.z = 0;
	endLocation = end.y * gridWidth + end.x;

	PathNode firstNode;
	firstNode.g = 0;
	firstNode.f = hEstimate;
	firstNode.px = (unsigned short)start.x;
	firstNode.py = (unsigned short)start.y;
	firstNode.pz = 0;
	firstNode.status = openNodeValue;

	if (costGrid[start.x][start.y - 1] == 0)
		firstNode.jumpLength = 0;
	else
		firstNode.jumpLength = (short)(maxCharacterJumpHeight * 2);

	nodes[location.xy].add(firstNode);
	touchedLocations.push(location.xy);

	open.Push(location);
	while (open.Count() > 0)//&& != stop)
	{
		location = open.Pop();

		if (nodes[location.xy][location.z].status == closedNodeValue)
			continue;

		locationX = (unsigned short)(location.xy % gridWidth);
		locationY = (unsigned short)(location.xy / gridWidth);

		if (location.xy == endLocation)
		{
			nodes[location.xy][location.z].status = closedNodeValue;
			found = true;
			break;
		}

		/*if (mCloseNodeCounter > mSearchLimit)
		{
			mStopped = true;
			return null;
		}*/

		/*if (mPunishChangeDirection)
			mHoriz = (mLocationX - mCalcGrid[mLocation].PX);*/

			//calculate neighbours
		for (int i = 0; i < (diagonals ? 8 : 4); i++)
		{
			newLocationX = (unsigned short)(locationX + directions[i][0]);
			newLocationY = (unsigned short)(locationY + directions[i][1]);
			newLocation = newLocationY * gridWidth + newLocationX;

			//check if we are inside the map bounderies
			if (newLocationX >= gridX || newLocationY >= gridY)
				continue;

			//check if the tile is walkable (if character bigger than 1 we had to check if the cahracter fits)
			if (costGrid[newLocationX][newLocationY] == 0) //TODO: maybe problems
				continue;

			bool onGround = false;
			bool atCeiling = false;

			//if (mMap.IsGround(newLocationX, newLocationY - 1)) 
			if (costGrid[newLocationX][newLocationY - 1] == 0)
				onGround = true;
			else if (costGrid[newLocationX][newLocationY + characterHeight] == 0)
				atCeiling = true;

			int jumpLength = nodes[location.xy][location.z].jumpLength;
			short newJumpLength = jumpLength;

			if (atCeiling)
			{
				if (newLocationX != locationX)
					newJumpLength = (short)MAX(maxCharacterJumpHeight * 2 + 1, jumpLength + 1);
				else
					newJumpLength = (short)MAX(maxCharacterJumpHeight * 2, jumpLength + 2);
			}
			else if (onGround)
				newJumpLength = 0;
			else if (newLocationY > locationY)
			{
				if (jumpLength < 2) //first jump is always two block up instead of one up and optionally one to either right or left
					newJumpLength = 3;
				else  if (jumpLength % 2 == 0)
					newJumpLength = (short)(jumpLength + 2);
				else
					newJumpLength = (short)(jumpLength + 1);
			}
			else if (newLocationY < locationY)
			{
				if (jumpLength % 2 == 0)
					newJumpLength = (short)MAX(maxCharacterJumpHeight * 2, jumpLength + 2);
				else
					newJumpLength = (short)MAX(maxCharacterJumpHeight * 2, jumpLength + 1);
			}
			else if (!onGround && newLocationX != locationX)
				newJumpLength = (short)(jumpLength + 1);

			if (jumpLength >= 0 && jumpLength % 2 != 0 && locationX != newLocationX)
				continue;

			//if we're falling and succeor's height is bigger than ours, skip that successor
			if (jumpLength >= maxCharacterJumpHeight * 2 && newLocationY > locationY)
				continue;

			if (newJumpLength >= maxCharacterJumpHeight * 2 + 6 && newLocationX != locationX && (newJumpLength - (maxCharacterJumpHeight * 2 + 6)) % 8 != 3)
				continue;

			/*if (diagonalsAdditionalCost && i > 3)
				newG = grid[location].g + (int)(costGrid[newLocationX, newLocationY] * 2.41);
			else
				newG = grid[location].g + costGrid[newLocationX, newLocationY];*/
			newG = nodes[location.xy][location.z].g + costGrid[newLocationX][newLocationY] + newJumpLength / 4;

			if (nodes[newLocation].count() > 0)
			{
				//32767 = short MaxValue
				int lowestJump = 32767;
				bool couldMoveSideways = false;
				for (int j = 0; j < nodes[newLocation].count(); ++j)
				{
					if (nodes[newLocation][j].jumpLength < lowestJump)
						lowestJump = nodes[newLocation][j].jumpLength;

					if (nodes[newLocation][j].jumpLength % 2 == 0 && nodes[newLocation][j].jumpLength < maxCharacterJumpHeight * 2 + 6)
						couldMoveSideways = true;
				}

				if (lowestJump <= newJumpLength && (newJumpLength % 2 != 0 || newJumpLength >= maxCharacterJumpHeight * 2 + 6 || couldMoveSideways))
					continue;
			}

			switch (formula) //TODO: poder treure el switch quan hagi trobat la millor i simplement posar la heuristica que va millor
			{
			case HeuristicFormula::Manhattan:
				h = hEstimate * (abs(newLocationX - end.x) + abs(newLocationY - end.y)); //TODO: en el codi original es multiplica per hestimate que es 2 al principi ??
				break;
			case HeuristicFormula::Euclidean:
				h = (int)(hEstimate * sqrt(((newLocationX - end.x) * (newLocationX - end.x))) + sqrt(((newLocationY - end.y) * (newLocationY - end.y))));
				break;
			case HeuristicFormula::EuclideanNoSqt:
				h = (int)(hEstimate * ((newLocationX - end.x) * (newLocationX - end.x)) + ((newLocationY - end.y) * (newLocationY - end.y)));
				break;
			}

			PathNode newNode;
			newNode.jumpLength = newJumpLength;
			newNode.px = locationX;
			newNode.py = locationY;
			newNode.pz = (unsigned short)location.z;
			newNode.g = newG;
			newNode.f = newG + h;
			newNode.status = openNodeValue;

			if (nodes[newLocation].count() == 0)
				touchedLocations.push(newLocation);

			nodes[newLocation].add(newNode);
			open.Push(Location(newLocation, nodes[newLocation].count() - 1));
		}
		//Adding the current node to the "closed list"
		/*closedNodeCounter++;
		grid[location].status = closedNodeValue;*/
	}
	//Get the time

	if (found)
	{
		close.clear();
		int posX = end.x;
		int posY = end.y;

		PathNode fPrevNodeTmp;
		PathNode fNodeTmp = nodes[endLocation][0];

		iPoint fNode = end;
		iPoint fPrevNode = end;

		unsigned short loc = (fNodeTmp.py * gridWidth) + fNodeTmp.px;

		while (fNode.x != fNodeTmp.px || fNode.y != fNodeTmp.py)
		{
			PathNode fNextNodeTmp = nodes[loc][fNodeTmp.pz];

			if ((close.count() == 0)
				|| (fNextNodeTmp.jumpLength != 0 && fNodeTmp.jumpLength == 0)
				|| (fNodeTmp.jumpLength == 3 && fPrevNodeTmp.jumpLength != 0)
				|| (fNodeTmp.jumpLength == 0 && fPrevNodeTmp.jumpLength != 0)
				|| (fNode.y > close[close.count() - 1].y && fNode.y > fNodeTmp.py)
				|| ((costGrid[fNode.x - 1][fNode.y] == 0 || costGrid[fNode.x + 1][fNode.y] == 0)
					&& fNode.y != close[close.count() - 1].y && fNode.x != close[close.count() - 1].x))
				close.add(fNode);

			fPrevNode = fNode;
			posX = fNodeTmp.px;
			posY = fNodeTmp.py;
			fPrevNodeTmp = fNodeTmp;
			fNodeTmp = fNextNodeTmp;
			loc = (fNodeTmp.py * gridWidth) + fNodeTmp.px;
			fNode = iPoint(posX, posY);
		}

		close.add(fNode);

		//mStopped = true;

		return true;
	}
}