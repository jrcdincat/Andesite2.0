#pragma once

class Layer {
public: 
	virtual ~Layer() {};
	virtual void Render() = 0; 
	virtual void Update() = 0; 
};