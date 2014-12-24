#include "ParticleSystem.hpp"
#include <fstream>
#include <iostream>

Constraint::Constraint(int f, int s, float l)
  : first(f), second(s), rest_length(l)
{}

ParticleSystem::ParticleSystem(const glm::vec2& min, const glm::vec2& max)
  : _timestep(0.005f), _nb_particles(0), _gravity(0, -4.81f), _min(min), _max(max)
{}

void ParticleSystem::read(const std::string& filename)
{
  clear();

  std::ifstream ifs(filename);
  if (ifs) {
    int n, a, b;
    float x, y, r;

    ifs >> n;
    for (int i = 0; i < n; ++i) {
      ifs >> x >> y;
      x += (2.0 * std::rand() / RAND_MAX - 1.0f) / 10.0f;
      y += (2.0 * std::rand() / RAND_MAX - 1.0f) / 10.0f;
      add_particle({ x, y });
    }

    ifs >> n;
    for (int i = 0; i < n; ++i) {
      ifs >> a >> b >> r;
      add_constraint({ a, b, r });
    }
  }

  step();
}

const std::vector<glm::vec2>& ParticleSystem::particles() const
{
  return _positions;
}

void ParticleSystem::add_particle(const glm::vec2& position)
{
  _positions.push_back(position);
  _old_positions.push_back(position);
  _forces.push_back(_gravity);
  ++_nb_particles;
}

const std::vector<Constraint>& ParticleSystem::constraints() const
{
  return _constraints;
}

void ParticleSystem::add_constraint(const Constraint& constraint)
{
  _constraints.push_back(constraint);
}

void ParticleSystem::clear()
{
  _positions.clear();
  _old_positions.clear();
  _forces.clear();
  _constraints.clear();
  _nb_particles = 0;
}

void ParticleSystem::step()
{
  accumulate_forces();
  verlet_integration();
  satisfy_constraints();
}

void ParticleSystem::verlet_integration()
{
  for (size_t i = 0; i < _nb_particles; ++i) {
    glm::vec2& pos = _positions[i];
    glm::vec2& old_pos = _old_positions[i];
    glm::vec2& acc = _forces[i];
    glm::vec2 tmp = pos;
    pos += pos - old_pos + _timestep * _timestep * acc;
    old_pos = tmp;
  }
}

void ParticleSystem::satisfy_constraints()
{
  for (int iter = 0; iter < 5; ++iter) {
    // stay inside the box
    for (size_t i = 0; i < _nb_particles; ++i) {
      glm::vec2& pos = _positions[i];
      pos.x = std::max(_min.x, std::min(_max.x, pos.x));
      pos.y = std::max(_min.y, std::min(_max.y, pos.y));
    }

    // relax constraints
    for (const auto& c : _constraints) {
      const glm::vec2 d = _positions[c.second] - _positions[c.first];
      const float len = glm::length(d);
      if (len < 0.001f) {
        _positions[c.first] += 0.01f;
        _positions[c.second] -= 0.01f;
      } else {
        float diff = (len - c.rest_length) / len;
        diff = (diff < 0 ? std::max(diff, -c.rest_length / 10.0f) : std::min(diff, c.rest_length / 10.0f));
        _positions[c.first] += 0.5f * diff * d;
        _positions[c.second] -= 0.5f * diff * d;
      }
    }
  }
}

void ParticleSystem::accumulate_forces()
{
  for (size_t i = 0; i < _nb_particles; ++i) {
    _forces[i] = _gravity;
  }
}
