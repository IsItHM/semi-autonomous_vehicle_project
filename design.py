import React from 'react';
import { AlignVertical, Sketching } from 'lucide-react';

const MarsRoverJacketDesign = () => {
  return (
    <div className="flex flex-col items-center p-4 bg-gray-100">
      <div className="w-64 h-96 bg-black text-white relative shadow-lg rounded-lg overflow-hidden">
        {/* Back Design */}
        <div className="absolute inset-0 flex items-center justify-center">
          <div className="text-white text-2xl font-bold tracking-wider text-center">
            UIU Mars Rover Team
          </div>
        </div>

        {/* Front Left Side - Bangladesh Flag and Text */}
        <div className="absolute top-4 left-4">
          <div className="w-16 h-10 bg-green-600 relative">
            <div className="absolute inset-0 flex items-center justify-center">
              <div className="w-8 h-8 bg-red-500 rounded-full"></div>
            </div>
          </div>
          <div className="text-white text-sm text-center mt-1">
            Bangladesh
          </div>
        </div>

        {/* Front Right Side - UMRT */}
        <div className="absolute top-4 right-4 text-white text-2xl font-bold">
          UMRT
        </div>

        {/* Left Arm - Mars Rover Arm Sketch */}
        <div className="absolute left-0 top-1/2 transform -translate-y-1/2 w-12 h-full">
          <svg viewBox="0 0 50 200" className="w-full h-full">
            <path 
              d="M10 0 L5 50 L25 100 L40 150 L45 200" 
              stroke="white" 
              strokeWidth="2" 
              fill="none"
            />
            <circle cx="25" cy="100" r="5" fill="white" />
          </svg>
        </div>

        {/* Right Arm - Rover Skeleton Sketch */}
        <div className="absolute right-0 top-1/2 transform -translate-y-1/2 w-12 h-full">
          <svg viewBox="0 0 50 200" className="w-full h-full">
            <path 
              d="M10 50 L25 0 L40 50 L30 100 L20 150 L10 200" 
              stroke="white" 
              strokeWidth="2" 
              fill="none"
            />
            <circle cx="25" cy="100" r="5" fill="white" />
          </svg>
        </div>
      </div>
    </div>
  );
};

export default MarsRoverJacketDesign;