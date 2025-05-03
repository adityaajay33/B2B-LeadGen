'use client';

import { useEffect, useRef, useState } from 'react';
import { Canvas, useThree } from '@react-three/fiber';
import { OrbitControls } from '@react-three/drei';
import * as THREE from 'three';

interface GraphData {
  nodes: Array<{ id: number; name: string; specificIndustry?: string; location?: string }>;
  links: Array<{ source: number; target: number; value: number }>;
}

interface Graph3DProps {
  data: GraphData;
}

function Tooltip({ x, y, company }: { x: number; y: number; company: any }) {
  return (
    <div
      style={{
        position: 'fixed',
        left: x + 16,
        top: y + 16,
        zIndex: 50,
        pointerEvents: 'none',
        minWidth: 200,
        maxWidth: 300,
      }}
      className="bg-[#1A1425] text-white rounded-lg shadow-lg p-4 border border-purple-500 text-sm"
    >
      <div className="font-bold text-base mb-1">{company.name}</div>
      {company.location && <div className="text-gray-400 mb-1">{company.location}</div>}
      {company.specificIndustry && <div className="text-purple-400">{company.specificIndustry}</div>}
    </div>
  );
}

function Graph({ data, onHover }: { data: GraphData; onHover: (id: number | null, mouse?: { x: number; y: number }) => void }) {
  const groupRef = useRef<THREE.Group>(null);

  useEffect(() => {
    if (!groupRef.current) return;

    // Position nodes in a 3D space
    const nodePositions = new Map();
    data.nodes.forEach((node, i) => {
      const angle = (i / data.nodes.length) * Math.PI * 2;
      const radius = 5;
      nodePositions.set(node.id, {
        x: Math.cos(angle) * radius,
        y: Math.sin(angle) * radius,
        z: (Math.random() - 0.5) * 2
      });
    });

    // Update positions of nodes in the group
    groupRef.current.children.forEach((child: THREE.Object3D) => {
      const nodeId = parseInt(child.name);
      const pos = nodePositions.get(nodeId);
      if (pos) {
        child.position.set(pos.x, pos.y, pos.z);
      }
    });
  }, [data]);

  return (
    <group ref={groupRef}>
      {data.nodes.map((node) => (
        <mesh
          key={node.id}
          name={node.id.toString()}
          onPointerOver={(e) => {
            e.stopPropagation();
            onHover(node.id, { x: e.clientX, y: e.clientY });
          }}
          onPointerMove={(e) => {
            onHover(node.id, { x: e.clientX, y: e.clientY });
          }}
          onPointerOut={() => {
            onHover(null);
          }}
        >
          <sphereGeometry args={[0.3, 32, 32]} />
          <meshStandardMaterial color="#8B5CF6" />
        </mesh>
      ))}
      {data.links.map((link, i) => {
        const sourcePos = groupRef.current?.getObjectByName(link.source.toString())?.position;
        const targetPos = groupRef.current?.getObjectByName(link.target.toString())?.position;
        if (!sourcePos || !targetPos) return null;

        return (
          <line key={i}>
            <bufferGeometry>
              <bufferAttribute
                attach="attributes-position"
                args={[new Float32Array([
                  sourcePos.x, sourcePos.y, sourcePos.z,
                  targetPos.x, targetPos.y, targetPos.z,
                ]), 3]}
              />
            </bufferGeometry>
            <lineBasicMaterial color="#A78BFA" opacity={1} transparent={false} />
          </line>
        );
      })}
    </group>
  );
}

export default function Graph3D({ data }: Graph3DProps) {
  const [hovered, setHovered] = useState<number | null>(null);
  const [mouse, setMouse] = useState<{ x: number; y: number } | null>(null);

  // Helper to get company info by node id
  const getCompany = (id: number) => data.nodes.find((n) => n.id === id);

  return (
    <>
      <Canvas camera={{ position: [0, 0, 15] }} style={{ width: '100%', height: '100%' }}>
        <ambientLight intensity={0.5} />
        <pointLight position={[10, 10, 10]} />
        <Graph
          data={data}
          onHover={(id, mousePos) => {
            setHovered(id);
            setMouse(mousePos || null);
          }}
        />
        <OrbitControls />
      </Canvas>
      {/* Tooltip overlay outside Canvas */}
      {hovered !== null && mouse && (
        <Tooltip x={mouse.x} y={mouse.y} company={getCompany(hovered) || {}} />
      )}
    </>
  );
}
