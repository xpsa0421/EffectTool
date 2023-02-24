#include "pch.h"
#include "ParticleSystem.h"

bool ParticleSystem::SetDeviceContext(ID3D11Device* device, ID3D11DeviceContext* context)
{
    device_ = device;
    device_context_ = context;
}

bool ParticleSystem::Init()
{
    // TODO: initialise variables

    // create default emitter if this is a new particle system
    if (emitters_.empty())
    {
        AddDefaultEmitter();
    }
}

/**
 * Update all emitters in the system
 */
bool ParticleSystem::Frame()
{
    for (ParticleEmitter* emitter : emitters_)
    {
        emitter->Frame();
    }
}

/**
 * Render all emitters in the system
 */
bool ParticleSystem::Render()
{
    for (ParticleEmitter* emitter : emitters_)
    {
        emitter->Render();
    }
}

/**
 * Release all emitters in the system
 */
bool ParticleSystem::Release()
{
    for (ParticleEmitter* emitter : emitters_)
    {
        emitter->Release();
        delete emitter;
    }
    emitters_.clear();
}

/**
 * Creates and adds a default emitter to the system
 */
void ParticleSystem::AddDefaultEmitter()
{
    ParticleEmitter* emitter = new ParticleEmitter;
    emitter->Init();
    emitter->SetName(L"ParticleEmitter");
    emitter->SetSpawnRate(20);
    emitter->SetEmitterPos({ 0,0,0 });
    emitter->SetSizeOffset({ 25,25 }, { 25,25 });
    emitter->SetLifetimeOffset(1, 1);
    emitter->SetVelocity({ -30,50,-10 }, { 30,100,10 });
    emitter->Create(device_.Get(), device_context_.Get());
    emitter->SetTexture(L"../../data/image/OrientParticle.PNG");
    emitters_.push_back(emitter);
}

/**
 * Loads and creates a particle system from a json text file
 *
 * @param	W_STR the path to the particle system json file
 * @returns	the loaded particle system if load succeeds
 *          nullptr if the load fails
 * 
 * TODO: Only load the required + customised properties of the emitters
 */
ParticleSystem* ParticleSystem::LoadFromFile(ID3D11Device* device, W_STR filepath)
{
    // retrieve immediate context from device
    ID3D11DeviceContext* device_context = nullptr;
    device->GetImmediateContext(&device_context);

    // create a particle system and set device and immediate context
    ParticleSystem* particle_system = new ParticleSystem();
    particle_system->SetDeviceContext(device, device_context);

    // emitter property variables
    W_STR               emitter_name;
    std::vector<W_STR>  tex_paths;
    int                 tex_cols;
    int                 tex_rows;
    float               spawn_rate;
    XMFLOAT3            emitter_pos;
    XMFLOAT3            pos_offset_min;
    XMFLOAT3            pos_offset_max;
    XMFLOAT2            initial_size_min;
    XMFLOAT2            initial_size_max;
    XMFLOAT2            lifetime;
    XMFLOAT3            velocity_min;
    XMFLOAT3            velocity_max;
    XMFLOAT4            color;

    // document to save particle system data
    rapidjson::Document particle_system_document;

    // load file and check validity
    if (!JsonHelper::LoadJSON(filepath, particle_system_document)) return false;

    // iterate all particle emitters in the system
    for (auto& emitter : particle_system_document.GetObject())
    {
        rapidjson::Value& properties = emitter.value;

        // check for incorrect emitter data
        if (!properties.IsObject() /* || properties.MemberCount() != EMITTER_PROPERTIES_COUNT*/)
        {
            return nullptr;
        }

        // retrieve emitter properties
        // retrieve number of rows in texture
        if (!JsonHelper::GetWString(properties, "name", emitter_name))
        {
            return nullptr;
        }

        // retrieve texture paths
        auto texpath_itr = properties.FindMember("tex_paths");
        if (texpath_itr == properties.MemberEnd())
        {
            return false;
        }
        rapidjson::Value& texpath_list = texpath_itr->value;
        if (!texpath_list.IsArray() || texpath_list.Size() == 0)
        {
            return nullptr;
        }
        for (rapidjson::SizeType i = 0; i < texpath_list.Size(); i++)
        {
            if (!texpath_list[i].IsString())
            {
                return nullptr;
            }
            tex_paths.push_back(mtw(texpath_list[i].GetString()));
        }

        // retrieve number of rows in texture
        if (!JsonHelper::GetInt(properties, "tex_rows", tex_rows))
        {
            return nullptr;
        }

        // retrieve number of rows in texture
        if (!JsonHelper::GetInt(properties, "tex_cols", tex_cols))
        {
            return nullptr;
        }

        // retrieve spawn rate
        if (!JsonHelper::GetFloat(properties, "spawn_rate", spawn_rate))
        {
            return nullptr;
        }

        // retrieve particle lifetime min and max
        if (!JsonHelper::GetFloat2(properties, "lifetime", lifetime))
        {
            return nullptr;
        }

        // retrieve emitter position
        if (!JsonHelper::GetFloat3(properties, "emitter_pos", emitter_pos))
        {
            return nullptr;
        }

        // retrieve minimum particle position offset 
        if (!JsonHelper::GetFloat3(properties, "pos_offset_min", pos_offset_min))
        {
            return nullptr;
        }

        // retrieve maximum particle position offset 
        if (!JsonHelper::GetFloat3(properties, "pos_offset_max", pos_offset_max))
        {
            return nullptr;
        }

        // retrieve minimum particle initial size
        if (!JsonHelper::GetFloat2(properties, "initial_size_min", initial_size_min))
        {
            return nullptr;
        }

        // retrieve maximum particle initial size
        if (!JsonHelper::GetFloat2(properties, "initial_size_max", initial_size_max))
        {
            return nullptr;
        }

        // retrieve minimum particle initial velocity
        if (!JsonHelper::GetFloat3(properties, "initial_velocity_min", velocity_min))
        {
            return nullptr;
        }

        // retrieve maximum particle initial velocity
        if (!JsonHelper::GetFloat3(properties, "initial_velocity_max", velocity_max))
        {
            return nullptr;
        }

        // retrieve particle color
        if (!JsonHelper::GetFloat4(properties, "color", color))
        {
            return nullptr;
        }


        // create a new particle emitter
        ParticleEmitter* emitter = new ParticleEmitter();
        emitter->Init();

        // set emitter properties from the loaded data
        if (tex_paths.size() > 1)
        {
            emitter->SetMultiTexAnimation(tex_paths);
        }
        else if ((tex_cols == 1) && (tex_rows == 1))
        {
            emitter->SetTexture(tex_paths[0]);
        }
        else
        {
            emitter->SetUVAnimation(tex_paths[0], tex_rows, tex_cols);
        }
        emitter->SetName(emitter_name);
        emitter->SetSpawnRate(spawn_rate);
        emitter->SetLifetimeOffset(lifetime.x, lifetime.y);
        emitter->SetEmitterPos(emitter_pos);
        emitter->SetPosOffset(pos_offset_min, pos_offset_max);
        emitter->SetSizeOffset(initial_size_min, initial_size_max);
        emitter->SetVelocity(velocity_max, velocity_max);
        //emitter->SetAdditiveColor();
        emitter->Create(device, device_context);

        // add loaded particle emitter to the system
        particle_system->emitters_.push_back(emitter);
    }

    return particle_system;
}