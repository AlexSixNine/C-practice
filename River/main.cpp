  cout << "====================== Question 1 ======================" << endl << endl;

  cout << "Recreating Figure 2: "<< endl;

  // the scene shown in Figure 2 in the spec 
  cout << "The scene shown in Figure 2:" << endl;
  scene = make_river_scene("CM","MM");
  print_scene(scene);  

  // the initial state 
  cout << "The initial state: "<< endl;
  scene = make_river_scene("BCCCMMM","");
  print_scene(scene);
  
  
  cout << "====================== Question 2 ======================" << endl << endl;
  
  char left[10] = "MMMB";
  int result = perform_crossing(left, "MM");
  cout << status_description(result) << endl << endl;
  cout << "After crossing left = " << left << endl;
  
  
  cout << "====================== Question 3 ======================" << endl << endl;

  result = play_game();

  cout << "The game ended as follows: " << status_description(result) << endl;

  /*
  cout << "=================== Bonus Challenge ====================" << endl << endl;

  char answer[512];
  strcpy(left, "MMMCCCB");
  if (find_solution(left, answer)==VALID_GOAL_STATE)
    cout << "Solution is " << answer << endl;
  else
    cout << "Solution does not exist." << endl;
  */
  return 0;
}
