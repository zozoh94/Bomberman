bomb* CreateBomb(player* p){
  //Changer rayon, timer et type en fonction du joueur
  return InitBomb(p->x, p->y, p->bombR, TIMERBOMB, p, p->bombT);
}
