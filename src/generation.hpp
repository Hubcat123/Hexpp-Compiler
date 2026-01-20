#pragma once

#include "parser.hpp"

#include <sstream>
#include <stack>

enum PatternType {
    akashas_distillation,
    akashas_gambit,
    additive_distillation,
    alidades_purification,
    anchorites_flight,
    archers_distillation,
    architects_distillation,
    arcs_reflection,
    augurs_exaltation,
    augurs_purification,
    auditors_purification,
    auditors_reflection,
    assessors_purification,
    assessors_reflection,
    aviators_purification,
    black_suns_nadir,
    black_suns_zenith,
    blue_suns_nadir,
    blue_suns_zenith,
    bookkeepers_gambit,
    break_block,
    charons_gambit,
    chroniclers_gambit,
    chroniclers_prfn,
    circle_reflection,
    compass_purification,
    compass_purification_II,
    conjunction_distillation,
    conjure_light,
    consideration,
    cosine_purification,
    create_water,
    derivation_decomposition,
    dioscuri_gambit,
    disjunction_distillation,
    division_distillation,
    dispel_rain,
    eulers_reflection,
    equality_distillation,
    evanition,
    explosion,
    false_reflection,
    fireball,
    floor_purification,
    fishermans_gambit,
    fishermans_gambit_II,
    flocks_disintegration,
    flocks_gambit,
    flocks_reflection,
    gemini_decomposition,
    gemini_gambit,
    greater_fold_reflection,
    hermes_gambit,
    huginns_gambit,
    inequality_distillation,
    integration_distillation,
    inverse_cosine_purification,
    inverse_sine_purification,
    inverse_tangent_distillation,
    introspection,
    iris_gambit,
    jesters_gambit,
    length_purification,
    locators_distillation,
    lodestone_reflection,
    logarithmic_distillation,
    make_note,
    maximus_distillation,
    maximus_distillation_II,
    minds_reflection,
    minimus_distillation,
    minimus_distillation_II,
    modulus_distillation,
    multiplicative_distillation,
    muninns_reflection,
    negation_purification,
    nullary_reflection,
    numerical_reflection,
    pace_purification,
    power_distillation,
    prospectors_gambit,
    retrospection,
    reveal,
    rotation_gambit,
    rotation_gambit_II,
    selection_distillation,
    selection_exaltation,
    speakers_decomposition,
    speakers_distillation,
    singles_purification,
    stadiometers_prfn,
    subtractive_distillation,
    sine_purification,
    swindlers_gambit,
    summon_lightning,
    summon_rain,
    surgeons_exaltation,
    thanatos_reflection,
    thoths_gambit,
    true_reflection,
    vacant_reflection,
    vector_disintegration,
    vector_exaltation,
    vector_reflection_NX,
    vector_reflection_NY,
    vector_reflection_NZ,
    vector_reflection_PX,
    vector_reflection_PY,
    vector_reflection_PZ,
    vector_reflection_zero,
    wayfarers_flight,
    waystone_reflection,
    white_suns_nadir,
    white_suns_zenith,
    num_patterns
};

struct Pattern {
    PatternType type;
    std::optional<std::string> value;
};



class Generator {
public:
    struct Var {
        std::string name;
        size_t stack_loc;
        bool is_global;
    };

    Generator(const NodeProg* root);

    std::vector<Pattern> generate();

    void gen_assignment(const NodeTermVar* var, const std::variant<const NodeExpr*, const float> value, TokenType_ op, size_t line, bool is_post = false);
    void gen_func(const NodeFunc* func);
    // Returns whether func is void func
    bool gen_defined_func(const NodeDefinedFunc* func);
    void gen_bin_expr(const NodeExprBin* expr_bin);
    // Returns the var of the variable gen-ed
    Var gen_var_ident(const std::string ident_name, size_t line, bool dont_gen_if_global = false, bool leave_copy = true);
    void gen_var(const NodeTermVar* var);
    void gen_term(const NodeTerm* term);
    void gen_expr(const NodeExpr* expr);
    void gen_stmt(const NodeStmt* stmt);
    void gen_func_def(const NodeFunctionDef* func_def);
    void gen_prog();
    
    void try_gen_x_exprs(std::vector<NodeExpr*> exprs, int correct_amount, size_t line);
    void pop(int amount = 1);
    void begin_scope();
    void end_scope();
    void end_scopes_return(bool has_ret_value);
    void dec_func(bool is_void, std::string name, int num_params, size_t line);

    void additive_distillation();
    void alidades_purification();
    void archers_distillation();
    void architects_distillation();
    void augurs_purification();
    void break_block();
    void compass_purification();
    void compass_purification_II();
    // AND
    void conjunction_distillation();
    void conjure_light();
    void create_water();
    void dioscuri_gambit();
    // OR
    void disjunction_distillation();
    void division_distillation();
    void equality_distillation();
    void false_reflection();
    void fishermans_gambit();
    void fishermans_gambit_II();
    void flocks_gambit(int num_iotas_packed);
    void flocks_reflection();
    void gemini_decomposition();
    // Store in Raven's Mind
    void huginns_gambit();
    void inequality_distillation();
    void integration_distillation();
    void jesters_gambit();
    void length_purification();
    void locators_distillation();
    void maximus_distillation();
    void maximus_distillation_II();
    void minds_reflection();
    void minimus_distillation();
    void minimus_distillation_II();
    void modulus_distillation();
    void multiplicative_distillation();
    // Retrieve from Raven's Mind
    void muninns_reflection();
    void negation_purification();
    void nullary_reflection();
    void numerical_reflection(std::string value);
    void power_distillation();
    void reveal();
    // 3rd becomes 1st
    void rotation_gambit();
    // 1st becomes 3rd
    void rotation_gambit_II();
    void singles_purification();
    void selection_distillation();
    void subtractive_distillation();
    void surgeons_exaltation();
    void true_reflection();
    void vacant_reflection();
    void vector_exaltation();

    void add_pattern(PatternType pattern, size_t stack_size_net, std::optional<std::string> value = std::nullopt);

    bool has_non_integer_num = false;

    bool generating_void_function = false;
private:

    struct Func {
        bool is_void;
        std::string name;
        int num_params;
        size_t stack_loc;
    };

    struct Scope {
        size_t stack_size;
        size_t var_num;
    };

    const NodeProg* m_prog;
    std::vector<Pattern> m_output;
    size_t m_stack_size = 0;
    std::vector<Var> m_vars {};
    std::vector<Var> m_global_vars {};
    std::vector<Func> m_funcs {};
    std::vector<Scope> m_scopes {};

    size_t m_function_start_scope;
    size_t m_function_num_params;
};