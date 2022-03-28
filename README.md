# 3DprojFQ
ah

left = traverseAST(node->children.at(0), scope);
            right = traverseAST(node->children.at(1), scope);
            if(left->type == "identifier"){
                left_record = symbolTable->lookUp(left->value);
            }
            else{
                left_record = new Record(left->value, left->type);
            }
            if(right->type == "identifier"){
                right_record = symbolTable->lookUp(right->value);
            }
            else{
                right_record = new Record(right->value, right->type);
            }
            typeChecking(left_record, right_record,node);
            Node* returnNode = new Node("","");
            if(node->value == "&&" || node->value == "||"){//TODO : check so both sides are boolean
                returnNode->type = "boolean";
                returnNode->value = left->value + node->value + right->value;
            }
            else if(node->value == "<" || node->value == ">"){
                //check so both are ints
                if(left_record->type == "int" && right_record->type == "int"){
                    returnNode->type = "boolean";
                    returnNode->value = left->value + node->value + right->value;
                }
                else{
                    if(left_record->type != "int"){
                    errorManager::add(Error {std::string(left_record->id + 
                    " cannot be a " + left_record->type),node});
                    }
                    if(right_record->type != "int"){
                        errorManager::add(Error {std::string(right_record->id + 
                        " cannot be a " + right_record->type),node});
                    }
                }
                
            }
            else if(node->value == "=="){//TODO : check so both sides are either int
                
            }
            else{
                if(left_record->type != "int"){
                    errorManager::add(Error {std::string(left_record->id + 
                    " cannot be a " + left_record->type),node});
                }
                if(right_record->type != "int"){
                    errorManager::add(Error {std::string(right_record->id + 
                    " cannot be a " + right_record->type),node});
                }
                //change this so array of int can also be here
                returnNode->type = "int";
                returnNode->value = left->value + node->value + right->value; //NOTE: I dont know what this does...
            }
            return returnNode;