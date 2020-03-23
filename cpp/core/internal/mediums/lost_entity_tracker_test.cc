#include "core/internal/mediums/lost_entity_tracker.h"

#include "platform/impl/default/default_platform.h"
#include "gtest/gtest.h"

namespace location {
namespace nearby {
namespace connections {
namespace mediums {
namespace {

struct TestEntity {
  int id;

  explicit TestEntity(int givenId) : id(givenId) {}

  bool operator<(const TestEntity &other) const { return id < other.id; }
};

TEST(LostEntityTracker, NoEntitiesLost) {
  LostEntityTracker<DefaultPlatform, TestEntity> lost_entity_tracker;
  ScopedPtr<ConstPtr<TestEntity> > entity_1(MakeConstPtr(new TestEntity(1)));
  ScopedPtr<ConstPtr<TestEntity> > entity_2(MakeConstPtr(new TestEntity(2)));
  ScopedPtr<ConstPtr<TestEntity> > entity_3(MakeConstPtr(new TestEntity(3)));

  // Discover some entities.
  lost_entity_tracker.recordFoundEntity(entity_1.get());
  lost_entity_tracker.recordFoundEntity(entity_2.get());
  lost_entity_tracker.recordFoundEntity(entity_3.get());

  // Make sure none are lost on the first round.
  ASSERT_TRUE(lost_entity_tracker.computeLostEntities().empty());

  // Rediscover the same entities.
  lost_entity_tracker.recordFoundEntity(entity_1.get());
  lost_entity_tracker.recordFoundEntity(entity_2.get());
  lost_entity_tracker.recordFoundEntity(entity_3.get());

  // Make sure we still didn't lose any entities.
  ASSERT_TRUE(lost_entity_tracker.computeLostEntities().empty());
}

TEST(LostEntityTracker, AllEntitiesLost) {
  LostEntityTracker<DefaultPlatform, TestEntity> lost_entity_tracker;
  ScopedPtr<ConstPtr<TestEntity> > entity_1(MakeConstPtr(new TestEntity(1)));
  ScopedPtr<ConstPtr<TestEntity> > entity_2(MakeConstPtr(new TestEntity(2)));
  ScopedPtr<ConstPtr<TestEntity> > entity_3(MakeConstPtr(new TestEntity(3)));

  // Discover some entities.
  lost_entity_tracker.recordFoundEntity(entity_1.get());
  lost_entity_tracker.recordFoundEntity(entity_2.get());
  lost_entity_tracker.recordFoundEntity(entity_3.get());

  // Make sure none are lost on the first round.
  ASSERT_TRUE(lost_entity_tracker.computeLostEntities().empty());

  // Go through a round without rediscovering any entities.
  typename LostEntityTracker<DefaultPlatform, TestEntity>::EntitySet
      lost_entities = lost_entity_tracker.computeLostEntities();
  ASSERT_TRUE(lost_entities.find(entity_1.get()) != lost_entities.end());
  ASSERT_TRUE(lost_entities.find(entity_2.get()) != lost_entities.end());
  ASSERT_TRUE(lost_entities.find(entity_3.get()) != lost_entities.end());
}

TEST(LostEntityTracker, SomeEntitiesLost) {
  LostEntityTracker<DefaultPlatform, TestEntity> lost_entity_tracker;
  ScopedPtr<ConstPtr<TestEntity> > entity_1(MakeConstPtr(new TestEntity(1)));
  ScopedPtr<ConstPtr<TestEntity> > entity_2(MakeConstPtr(new TestEntity(2)));
  ScopedPtr<ConstPtr<TestEntity> > entity_3(MakeConstPtr(new TestEntity(3)));

  // Discover some entities.
  lost_entity_tracker.recordFoundEntity(entity_1.get());
  lost_entity_tracker.recordFoundEntity(entity_2.get());

  // Make sure none are lost on the first round.
  ASSERT_TRUE(lost_entity_tracker.computeLostEntities().empty());

  // Go through the next round only rediscovering one of our entities and
  // discovering an additional entity as well. Then, verify that only one entity
  // was lost after the check.
  lost_entity_tracker.recordFoundEntity(entity_1.get());
  lost_entity_tracker.recordFoundEntity(entity_3.get());
  typename LostEntityTracker<DefaultPlatform, TestEntity>::EntitySet
      lost_entities = lost_entity_tracker.computeLostEntities();
  ASSERT_TRUE(lost_entities.find(entity_1.get()) == lost_entities.end());
  ASSERT_TRUE(lost_entities.find(entity_2.get()) != lost_entities.end());
  ASSERT_TRUE(lost_entities.find(entity_3.get()) == lost_entities.end());
}

TEST(LostEntityTracker, SameEntityMultipleCopies) {
  LostEntityTracker<DefaultPlatform, TestEntity> lost_entity_tracker;
  ScopedPtr<ConstPtr<TestEntity> > entity_1(MakeConstPtr(new TestEntity(1)));
  ScopedPtr<ConstPtr<TestEntity> > entity_1_copy(
      MakeConstPtr(new TestEntity(1)));

  // Discover an entity.
  lost_entity_tracker.recordFoundEntity(entity_1.get());

  // Make sure none are lost on the first round.
  ASSERT_TRUE(lost_entity_tracker.computeLostEntities().empty());

  // Rediscover the same entity, but through a copy of it.
  lost_entity_tracker.recordFoundEntity(entity_1_copy.get());

  // Make sure none are lost on the second round.
  ASSERT_TRUE(lost_entity_tracker.computeLostEntities().empty());

  // Go through a round without rediscovering any entities and verify that we
  // lost an entity equivalent to both copies of it.
  typename LostEntityTracker<DefaultPlatform, TestEntity>::EntitySet
      lost_entities = lost_entity_tracker.computeLostEntities();
  ASSERT_EQ(lost_entities.size(), 1);
  ASSERT_TRUE(lost_entities.find(entity_1.get()) != lost_entities.end());
  ASSERT_TRUE(lost_entities.find(entity_1_copy.get()) != lost_entities.end());
}

}  // namespace
}  // namespace mediums
}  // namespace connections
}  // namespace nearby
}  // namespace location
