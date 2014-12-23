#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Constraint
{
  Constraint(int f, int s, float l);

  int first;
  int second;
  float rest_length;
};

class ParticleSystem
{
public:
  ParticleSystem(const glm::vec2& min, const glm::vec2& max);

  void read(const std::string& filename);
  void step();

  void add_particle(const glm::vec2& position);
  const std::vector<glm::vec2>& particles() const;

  void add_constraint(const Constraint& constraint);
  const std::vector<Constraint>& constraints() const;

private:
  void clear();
  void verlet_integration();
  void satisfy_constraints();
  void accumulate_forces();

private:
  float _timestep;
  size_t _nb_particles;
  std::vector<glm::vec2> _positions;
  std::vector<glm::vec2> _old_positions;
  std::vector<glm::vec2> _forces;
  std::vector<Constraint> _constraints;
  glm::vec2 _gravity;
  glm::vec2 _min, _max;
};
