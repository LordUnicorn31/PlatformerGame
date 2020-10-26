#include "Player.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Draw() {

}

void Player::Move() {
	//Primer movem en x i despres en y
	// La variacio en la speed depen del input que li dona el player i de la acceleracio (friccio / gravetat) (i si ha xocat?)

	//1. Calcular el moviment k tocaria si no hi ha cap obstacle
	// Afegir el calcul de la acceleracio (gravetat i acceleracio en x)
	// El dt? (Haig de ficar )

	//2. Agafar la coordenada de la bounding box en la direccio on ens estem movent

	//3. Mirar amb quines tiles intersecciona aquest edge de la bounding box

		//Per la layer ground -> layer->Get(app->map->worldtomap(collider.pos.x).x,app->map->worldtomap(collider.pos.y).y);

	//4.Buscar en la direccio d'aquestes tiles (row o columne a partir de la tile interseccio) el obstacle mes proper
	// Si hi ha moving obstacles ferne loop i mirar el mes proper
	// Tenir en compte que hi ha one way tiles que nomes afecten a la y i nomes si el player hi esta a sobre

		/* Get Tile Properties from a gid example
	if(app->map->GetTileProperty(id,"Blocked"))
		LOG("TRUE");
	else {
		LOG("FALSE");
	}
	*/
	
	//5.Player movement = MIN(Closest distance, Calculated Movement) / Pos += Movement

	//6. Repetir el process pel moviment en l'altre coordenada
}

void Player::Update() {
	//Get the input and update the movement variables accordingly

	//Get the tiles under the player and check for some possible interactions (Ns si aixo s'auria de posar en el update logic)
}

void Player::UpdateLogic() {
	Move();
}