// Copyright (c) Andrew Fischer. See LICENSE file for license terms.

#pragma once

namespace circa {

struct World {

    Block* root;
    Block* builtins;

    // Private data.
    NativePatchWorld* nativePatchWorld;
    FileWatchWorld* fileWatchWorld;

    // Global IDs.
    int nextActorID;
    int nextTermID;
    int nextBlockID;
    int nextStackID;

    Type* firstPermanentType;
    Type* lastPermanentType;

    // Module information.
    List moduleSearchPaths;

    Stack* firstRootStack;
    Stack* lastRootStack;

    // Whether the world is currently bootstrapping. Either :Bootstrapping or :Done.
    Symbol bootstrapStatus;

    caLogFunc logFunc;
    void* logContext;

protected:
    // Disallow C++ construction
    World();
    ~World();
};

World* alloc_world();
void dealloc_world(World* world);
void world_initialize(World* world);
World* create_world();

void refresh_all_modules(caWorld* world);

} // namespace circa
