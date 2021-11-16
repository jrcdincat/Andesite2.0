#pragma once

class Layer {
	public: 
		virtual ~Layer() {};
		virtual void Render() = 0; 
};